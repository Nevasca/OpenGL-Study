#include "LightingSystem.h"

#include <iostream>

#include "Shader.h"
#include "core/Basics/Components/CameraComponent.h"
#include "core/Basics/Components/DirectionalLightComponent.h"

void LightingSystem::AddDirectionalLight(const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent)
{
    m_DirectionalLights.push_back(directionalLightComponent);
    
    if(m_DirectionalLights.size() > MAX_DIRECTIONAL_LIGHTS)
    {
        // TODO: replace for log class when implemented
        std::cout << "Max directional lights reached. Last added directional light won't affect world\n";
    }
}

void LightingSystem::SetLightsFor(Shader& boundShader, const CameraComponent& activeCamera) const
{
    boundShader.SetUniform3f("u_AmbientLight.color", m_AmbientLightColor);
    boundShader.SetUniform3f("u_ViewPosition", activeCamera.GetOwnerPosition());

    const int totalDirectionalLights = std::min(static_cast<int>(m_DirectionalLights.size()), MAX_DIRECTIONAL_LIGHTS); 

    for(int i = 0; i < totalDirectionalLights; i++)
    {
        // TODO we could try testing performance if light is dirty and skip updating uniform if not

        boundShader.SetUniform1i("u_TotalDirectionalLights", totalDirectionalLights);

        auto& directionalLight = m_DirectionalLights[i];
        boundShader.SetUniform3f("u_DirectionalLights[" + std::to_string(i) + "].direction", directionalLight->GetDirection());
        boundShader.SetUniform3f("u_DirectionalLights[" + std::to_string(i) + "].diffuse", directionalLight->GetColor());
        boundShader.SetUniform1f("u_DirectionalLights[" + std::to_string(i) + "].intensity", directionalLight->GetIntensity());
        boundShader.SetUniform3f("u_DirectionalLights[" + std::to_string(i) + "].specular", m_DefaultSpecularColor);
    }
}
