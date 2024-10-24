#include "LightingSystem.h"

#include <iostream>

#include "Shader.h"
#include "core/Basics/Components/CameraComponent.h"
#include "core/Basics/Components/DirectionalLightComponent.h"
#include "core/Basics/Components/PointLightComponent.h"
#include "core/Basics/Components/SpotLightComponent.h"

LightingSystem::LightingSystem()
{
    CreateUniformBuffers();

    const Rendering::Resolution DEFAULT_SHADOW_RESOLUTION{1024, 1024};
    CreateShadowDepthMap(DEFAULT_SHADOW_RESOLUTION);
}

void LightingSystem::Setup()
{
    std::shared_ptr<Texture> depthTexture = m_ShadowDepthMapBuffer->GetDepthBufferTexture();
    depthTexture->Bind(SHADOW_MAP_SLOT);
}

void LightingSystem::Shutdown()
{
    m_DirectionalLights.clear();
    m_PointLights.clear();
    m_SpotLights.clear();

    std::shared_ptr<Texture> depthTexture = m_ShadowDepthMapBuffer->GetDepthBufferTexture();
    depthTexture->Unbind(SHADOW_MAP_SLOT);
    m_ShadowDepthMapBuffer.reset();
}

void LightingSystem::AddDirectionalLight(const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent)
{
    m_DirectionalLights.push_back(directionalLightComponent);
    
    if(m_DirectionalLights.size() > Rendering::MAX_DIRECTIONAL_LIGHTS)
    {
        // TODO: replace for log class when implemented
        std::cout << "Max directional lights reached. Last added directional light won't affect world\n";
    }

    m_TotalActiveDirectionalLights = std::min(static_cast<int>(m_DirectionalLights.size()), Rendering::MAX_DIRECTIONAL_LIGHTS);
}

void LightingSystem::AddPointLight(const std::shared_ptr<PointLightComponent>& pointLightComponent)
{
    m_PointLights.push_back(pointLightComponent);

    if(m_PointLights.size() > Rendering::MAX_POINT_LIGHTS)
    {
        // TODO: replace for log class when implemented
        std::cout << "Max point lights reached. Last added point light won't affect world\n";
    }

    m_TotalActivePointLights = std::min(static_cast<int>(m_PointLights.size()), Rendering::MAX_POINT_LIGHTS);
}

void LightingSystem::AddSpotLight(const std::shared_ptr<SpotLightComponent>& spotLightComponent)
{
    m_SpotLights.push_back(spotLightComponent);

    if(m_SpotLights.size() > Rendering::MAX_SPOT_LIGHTS)
    {
        // TODO: replace for log class when implemented
        std::cout << "Max spot lights reached. Last added spot light won't affect world\n";
    }

    m_TotalActiveSpotLights = std::min(static_cast<int>(m_SpotLights.size()), Rendering::MAX_SPOT_LIGHTS);
}

void LightingSystem::SetupUniformsFor(Shader& shader) const
{
    m_GeneralUniformBuffer->SetBindingIndexFor(shader);
    m_DirectionalUniformBuffer->SetBindingIndexFor(shader);
    m_PointUniformBuffer->SetBindingIndexFor(shader);
    m_SpotsUniformBuffer->SetBindingIndexFor(shader);
    m_DirectionalMatrixUniformBuffer->SetBindingIndexFor(shader);

    shader.Bind();
    shader.SetUniform1i("u_ShadowMap", SHADOW_MAP_SLOT);
    shader.Unbind();
}

void LightingSystem::UpdateLightingUniformBuffer(const CameraComponent& activeCamera)
{
    // TODO we could try testing performance if light is dirty and skip updating uniform if not

    m_GeneralShaderData.ViewPosition = activeCamera.GetOwnerPosition();
    m_GeneralShaderData.AmbientLight.Color = m_AmbientLightColor;

    m_GeneralShaderData.TotalDirectionalLights = m_TotalActiveDirectionalLights;
    
    for(int i = 0; i < m_TotalActiveDirectionalLights; i++)
    {
        auto& directionalLight = m_DirectionalLights[i];
    
        Rendering::DirectionalLightShaderData directionalLightShaderData;
        directionalLightShaderData.Intensity = directionalLight->GetIntensity();
        directionalLightShaderData.Bias = directionalLight->GetBias();
        directionalLightShaderData.NormalBias = directionalLight->GetNormalBias();
        directionalLightShaderData.Direction = directionalLight->GetDirection();
        directionalLightShaderData.Diffuse = directionalLight->GetColor();
        directionalLightShaderData.Specular = m_DefaultSpecularColor;
        
        m_DirectionalsShaderData[i] = directionalLightShaderData;
    }
    
    m_GeneralShaderData.TotalPointLights = m_TotalActivePointLights;
    
    for(int i = 0; i < m_TotalActivePointLights; i++)
    {
        auto& pointLight = m_PointLights[i];
        const Attenuation attenuation = pointLight->GetAttenuation();
    
        Rendering::PointLightShaderData pointLightShaderData;
        pointLightShaderData.Position = pointLight->GetPosition();
        pointLightShaderData.Diffuse = pointLight->GetColor();
        pointLightShaderData.Intensity = pointLight->GetIntensity();
        pointLightShaderData.Constant = attenuation.Constant;
        pointLightShaderData.Linear = attenuation.Linear;
        pointLightShaderData.Quadratic = attenuation.Quadratic;
        pointLightShaderData.Specular = m_DefaultSpecularColor;
    
        m_PointsShaderData[i] = pointLightShaderData;
    }
    
    m_GeneralShaderData.TotalSpotLights = m_TotalActiveSpotLights;
    
    for(int i = 0; i < m_TotalActiveSpotLights; i++)
    {
        auto& spotLight = m_SpotLights[i];
        const float cutoff = glm::cos(glm::radians(spotLight->GetInnerCutoffDegrees()));
        const float outerCutoff = glm::cos(glm::radians(spotLight->GetOuterCutoffDegrees()));
        const Attenuation attenuation = spotLight->GetAttenuation();
    
        Rendering::SpotLightShaderData spotLightShaderData;
        spotLightShaderData.Position = spotLight->GetPosition();
        spotLightShaderData.Direction = spotLight->GetDirection();
        spotLightShaderData.Cutoff = cutoff;
        spotLightShaderData.OuterCutoff = outerCutoff;
        spotLightShaderData.Diffuse = spotLight->GetColor();
        spotLightShaderData.Intensity = spotLight->GetIntensity();
        spotLightShaderData.Constant = attenuation.Constant;
        spotLightShaderData.Linear = attenuation.Linear;
        spotLightShaderData.Quadratic = attenuation.Quadratic;
        spotLightShaderData.Specular = m_DefaultSpecularColor;
    
        m_SpotsShaderData[i] = spotLightShaderData;
    }

    m_GeneralUniformBuffer->Bind();
    m_GeneralUniformBuffer->SetSubData(&m_GeneralShaderData, sizeof(Rendering::LightingGeneralShaderData));
    m_GeneralUniformBuffer->Unbind();

    m_DirectionalUniformBuffer->Bind();
    m_DirectionalUniformBuffer->SetSubData(m_DirectionalsShaderData, Rendering::MAX_DIRECTIONAL_LIGHTS * sizeof(Rendering::DirectionalLightShaderData));
    m_DirectionalUniformBuffer->Unbind();

    m_PointUniformBuffer->Bind();
    m_PointUniformBuffer->SetSubData(m_PointsShaderData, Rendering::MAX_POINT_LIGHTS * sizeof(Rendering::PointLightShaderData));
    m_PointUniformBuffer->Unbind();

    m_SpotsUniformBuffer->Bind();
    m_SpotsUniformBuffer->SetSubData(m_SpotsShaderData, Rendering::MAX_SPOT_LIGHTS * sizeof(Rendering::SpotLightShaderData));
    m_SpotsUniformBuffer->Unbind();

    std::shared_ptr<DirectionalLightComponent> mainDirectionalLight = GetMainDirectionalLight();

    if(!mainDirectionalLight)
    {
        return;
    }
    
    glm::mat4 lightSpaceMatrix = mainDirectionalLight->GetViewProjectionMatrix();
    m_DirectionalMatrixUniformBuffer->Bind();
    m_DirectionalMatrixUniformBuffer->SetSubData(&lightSpaceMatrix, sizeof(glm::mat4));
    m_DirectionalMatrixUniformBuffer->Unbind();
}

std::shared_ptr<DirectionalLightComponent> LightingSystem::GetMainDirectionalLight()
{
    if(m_DirectionalLights.empty())
    {
        return nullptr;
    }

    return m_DirectionalLights[0];
}

void LightingSystem::CreateUniformBuffers()
{
    constexpr unsigned int UNIFORM_LIGHTING_GENERAL_BINDING_INDEX = 2;
    constexpr unsigned int UNIFORM_LIGHTING_DIRECTIONALS_BINDING_INDEX = 3;
    constexpr unsigned int UNIFORM_LIGHTING_POINTS_BINDING_INDEX = 4;
    constexpr unsigned int UNIFORM_LIGHTING_SPOTS_BINDING_INDEX = 5;
    constexpr unsigned int UNIFORM_LIGHTING_DIRECTIONAL_MATRIX_BINDING_INDEX = 6;

    m_GeneralUniformBuffer = std::make_unique<Rendering::UniformBuffer>(
        nullptr,
        sizeof(Rendering::LightingGeneralShaderData),
        UNIFORM_LIGHTING_GENERAL_BINDING_INDEX,
        "LightingGeneral",
        true);

    m_DirectionalUniformBuffer = std::make_unique<Rendering::UniformBuffer>(
        nullptr,
        Rendering::MAX_DIRECTIONAL_LIGHTS * sizeof(Rendering::DirectionalLightShaderData),
        UNIFORM_LIGHTING_DIRECTIONALS_BINDING_INDEX,
        "LightingDirectionals",
        true);

    m_PointUniformBuffer = std::make_unique<Rendering::UniformBuffer>(
        nullptr,
        Rendering::MAX_POINT_LIGHTS * sizeof(Rendering::PointLightShaderData),
        UNIFORM_LIGHTING_POINTS_BINDING_INDEX,
        "LightingPoints",
        true);

    m_SpotsUniformBuffer = std::make_unique<Rendering::UniformBuffer>(
        nullptr,
        Rendering::MAX_SPOT_LIGHTS * sizeof(Rendering::SpotLightShaderData),
        UNIFORM_LIGHTING_SPOTS_BINDING_INDEX,
        "LightingSpots",
        true);

    m_DirectionalMatrixUniformBuffer = std::make_unique<Rendering::UniformBuffer>(
        nullptr,
        sizeof(glm::mat4),
        UNIFORM_LIGHTING_DIRECTIONAL_MATRIX_BINDING_INDEX,
        "LightMatrix",
        true);
}

void LightingSystem::CreateShadowDepthMap(const Rendering::Resolution& resolution)
{
    FramebufferSettings settings{};
    settings.Resolution = resolution;
    settings.EnableDepthMapOnly = true;
    
    m_ShadowDepthMapBuffer = std::make_unique<Framebuffer>(settings);
    m_ShadowDepthMapBuffer->SetClearColor(glm::vec4{1.f, 0.f, 1.f, 1.f});
}
