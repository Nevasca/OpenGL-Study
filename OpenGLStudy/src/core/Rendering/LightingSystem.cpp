#include "LightingSystem.h"

#include <iostream>

#include "Shader.h"
#include "core/Basics/Components/CameraComponent.h"
#include "core/Basics/Components/DirectionalLightComponent.h"
#include "core/Basics/Components/PointLightComponent.h"

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

void LightingSystem::SetLightsFor(Shader& boundShader, const CameraComponent& activeCamera) const
{
    boundShader.SetUniform3f("u_AmbientLight.color", m_AmbientLightColor);
    boundShader.SetUniform3f("u_ViewPosition", activeCamera.GetOwnerPosition());

    // TODO we could try testing performance if light is dirty and skip updating uniform if not

    for(int i = 0; i < m_TotalActiveDirectionalLights; i++)
    {
        boundShader.SetUniform1i("u_TotalDirectionalLights", m_TotalActiveDirectionalLights);

        auto& directionalLight = m_DirectionalLights[i];
        boundShader.SetUniform3f("u_DirectionalLights[" + std::to_string(i) + "].direction", directionalLight->GetDirection());
        boundShader.SetUniform3f("u_DirectionalLights[" + std::to_string(i) + "].diffuse", directionalLight->GetColor());
        boundShader.SetUniform1f("u_DirectionalLights[" + std::to_string(i) + "].intensity", directionalLight->GetIntensity());
        boundShader.SetUniform3f("u_DirectionalLights[" + std::to_string(i) + "].specular", m_DefaultSpecularColor);
    }

    for(int i = 0; i < m_TotalActivePointLights; i++)
    {
        boundShader.SetUniform1i("u_TotalPointLights", m_TotalActivePointLights);

        auto& pointLight = m_PointLights[i];
        const PointLightComponent::Attenuation attenuation = pointLight->GetAttenuation();

        boundShader.SetUniform3f("u_PointLights[" + std::to_string(i) + "].position", pointLight->GetPosition());
        boundShader.SetUniform3f("u_PointLights[" + std::to_string(i) + "].diffuse", pointLight->GetColor());
        boundShader.SetUniform1f("u_PointLights[" + std::to_string(i) + "].intensity", pointLight->GetIntensity());
        boundShader.SetUniform1f("u_PointLights[" + std::to_string(i) + "].constant", attenuation.Constant);
        boundShader.SetUniform1f("u_PointLights[" + std::to_string(i) + "].linear", attenuation.Linear);
        boundShader.SetUniform1f("u_PointLights[" + std::to_string(i) + "].quadratic", attenuation.Quadratic);
        boundShader.SetUniform3f("u_PointLights[" + std::to_string(i) + "].specular", m_DefaultSpecularColor);
    }
}
