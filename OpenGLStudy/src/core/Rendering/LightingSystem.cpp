#include "LightingSystem.h"

#include <iostream>

#include "Shader.h"
#include "core/Basics/Components/CameraComponent.h"
#include "core/Basics/Components/DirectionalLightComponent.h"
#include "core/Basics/Components/PointLightComponent.h"
#include "core/Basics/Components/SpotLightComponent.h"

void LightingSystem::Shutdown()
{
    m_DirectionalLights.clear();
    m_PointLights.clear();
    m_SpotLights.clear();
}

void LightingSystem::AddDirectionalLight(const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent)
{
    m_DirectionalLights.push_back(directionalLightComponent);
    
    if(m_DirectionalLights.size() > MAX_DIRECTIONAL_LIGHTS)
    {
        // TODO: replace for log class when implemented
        std::cout << "Max directional lights reached. Last added directional light won't affect world\n";
    }

    m_TotalActiveDirectionalLights = std::min(static_cast<int>(m_DirectionalLights.size()), MAX_DIRECTIONAL_LIGHTS);
}

void LightingSystem::AddPointLight(const std::shared_ptr<PointLightComponent>& pointLightComponent)
{
    m_PointLights.push_back(pointLightComponent);

    if(m_PointLights.size() > MAX_POINT_LIGHTS)
    {
        // TODO: replace for log class when implemented
        std::cout << "Max point lights reached. Last added point light won't affect world\n";
    }

    m_TotalActivePointLights = std::min(static_cast<int>(m_PointLights.size()), MAX_POINT_LIGHTS);
}

void LightingSystem::AddSpotLight(const std::shared_ptr<SpotLightComponent>& spotLightComponent)
{
    m_SpotLights.push_back(spotLightComponent);

    if(m_SpotLights.size() > MAX_SPOT_LIGHTS)
    {
        // TODO: replace for log class when implemented
        std::cout << "Max spot lights reached. Last added spot light won't affect world\n";
    }

    m_TotalActiveSpotLights = std::min(static_cast<int>(m_SpotLights.size()), MAX_SPOT_LIGHTS);
}

void LightingSystem::SetLightsFor(Shader& boundShader, const CameraComponent& activeCamera) const
{
    boundShader.SetUniform3f("u_AmbientLight.color", m_AmbientLightColor);
    boundShader.SetUniform3f("u_ViewPosition", activeCamera.GetOwnerPosition());

    // TODO we could try testing performance if light is dirty and skip updating uniform if not

    boundShader.SetUniform1i("u_TotalDirectionalLights", m_TotalActiveDirectionalLights);

    for(int i = 0; i < m_TotalActiveDirectionalLights; i++)
    {
        auto& directionalLight = m_DirectionalLights[i];
        boundShader.SetUniform3f("u_DirectionalLights[" + std::to_string(i) + "].direction", directionalLight->GetDirection());
        boundShader.SetUniform3f("u_DirectionalLights[" + std::to_string(i) + "].diffuse", directionalLight->GetColor());
        boundShader.SetUniform1f("u_DirectionalLights[" + std::to_string(i) + "].intensity", directionalLight->GetIntensity());
        boundShader.SetUniform3f("u_DirectionalLights[" + std::to_string(i) + "].specular", m_DefaultSpecularColor);
    }

    boundShader.SetUniform1i("u_TotalPointLights", m_TotalActivePointLights);
    
    for(int i = 0; i < m_TotalActivePointLights; i++)
    {
        auto& pointLight = m_PointLights[i];
        const Attenuation attenuation = pointLight->GetAttenuation();

        boundShader.SetUniform3f("u_PointLights[" + std::to_string(i) + "].position", pointLight->GetPosition());
        boundShader.SetUniform3f("u_PointLights[" + std::to_string(i) + "].diffuse", pointLight->GetColor());
        boundShader.SetUniform1f("u_PointLights[" + std::to_string(i) + "].intensity", pointLight->GetIntensity());
        boundShader.SetUniform1f("u_PointLights[" + std::to_string(i) + "].constant", attenuation.Constant);
        boundShader.SetUniform1f("u_PointLights[" + std::to_string(i) + "].linear", attenuation.Linear);
        boundShader.SetUniform1f("u_PointLights[" + std::to_string(i) + "].quadratic", attenuation.Quadratic);
        boundShader.SetUniform3f("u_PointLights[" + std::to_string(i) + "].specular", m_DefaultSpecularColor);
    }

    boundShader.SetUniform1i("u_TotalSpotLights", m_TotalActiveSpotLights);

    for(int i = 0; i < m_TotalActiveSpotLights; i++)
    {
        auto& spotLight = m_SpotLights[i];
        const float cutoff = glm::cos(glm::radians(spotLight->GetInnerCutoffDegrees()));
        const float outerCutoff = glm::cos(glm::radians(spotLight->GetOuterCutoffDegrees()));
        const Attenuation attenuation = spotLight->GetAttenuation();

        boundShader.SetUniform3f("u_SpotLights[" + std::to_string(i) + "].position", spotLight->GetPosition());
        boundShader.SetUniform3f("u_SpotLights[" + std::to_string(i) + "].direction", spotLight->GetDirection());
        boundShader.SetUniform1f("u_SpotLights[" + std::to_string(i) + "].cutoff", cutoff);
        boundShader.SetUniform1f("u_SpotLights[" + std::to_string(i) + "].outerCutoff", outerCutoff);
        boundShader.SetUniform3f("u_SpotLights[" + std::to_string(i) + "].diffuse", spotLight->GetColor());
        boundShader.SetUniform1f("u_SpotLights[" + std::to_string(i) + "].intensity", spotLight->GetIntensity());
        boundShader.SetUniform1f("u_SpotLights[" + std::to_string(i) + "].constant", attenuation.Constant);
        boundShader.SetUniform1f("u_SpotLights[" + std::to_string(i) + "].linear", attenuation.Linear);
        boundShader.SetUniform1f("u_SpotLights[" + std::to_string(i) + "].quadratic", attenuation.Quadratic);
        boundShader.SetUniform3f("u_SpotLights[" + std::to_string(i) + "].specular", m_DefaultSpecularColor);
    }
}
