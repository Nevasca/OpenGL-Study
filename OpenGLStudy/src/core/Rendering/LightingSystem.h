#pragma once
#include <memory>
#include <vector>

#include "glm/vec3.hpp"

class CameraComponent;
class Shader;
class DirectionalLightComponent;

class LightingSystem
{
public:

    void AddDirectionalLight(const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent);
    void SetLightsFor(Shader& boundShader, const CameraComponent& activeCamera) const;

    void SetAmbientLightColor(const glm::vec3& ambientLightColor) { m_AmbientLightColor = ambientLightColor; }

private:

    static constexpr int MAX_DIRECTIONAL_LIGHTS = 3;
    
    std::vector<std::shared_ptr<DirectionalLightComponent>> m_DirectionalLights{};
    glm::vec3 m_AmbientLightColor{0.05f, 0.15f, 0.175f};
    glm::vec3 m_DefaultSpecularColor{1.f};
};
