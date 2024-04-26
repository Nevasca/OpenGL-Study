#pragma once
#include <memory>
#include <vector>

#include "glm/vec3.hpp"

class SpotLightComponent;
class PointLightComponent;
class CameraComponent;
class Shader;
class DirectionalLightComponent;

class LightingSystem
{
public:

    void Shutdown();
    
    void AddDirectionalLight(const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent);
    void AddPointLight(const std::shared_ptr<PointLightComponent>& pointLightComponent);
    void AddSpotLight(const std::shared_ptr<SpotLightComponent>& spotLightComponent);
    void SetLightsFor(Shader& boundShader, const CameraComponent& activeCamera) const;

    void SetAmbientLightColor(const glm::vec3& ambientLightColor) { m_AmbientLightColor = ambientLightColor; }
    glm::vec3 GetAmbientLightColor() const { return m_AmbientLightColor; }

private:

    static constexpr int MAX_DIRECTIONAL_LIGHTS = 3;
    static constexpr int MAX_POINT_LIGHTS = 20;
    static constexpr int MAX_SPOT_LIGHTS = 20;
    
    std::vector<std::shared_ptr<DirectionalLightComponent>> m_DirectionalLights{};
    std::vector<std::shared_ptr<PointLightComponent>> m_PointLights{};
    std::vector<std::shared_ptr<SpotLightComponent>> m_SpotLights{};
    glm::vec3 m_AmbientLightColor{0.05f, 0.15f, 0.175f};
    glm::vec3 m_DefaultSpecularColor{1.f};
    int m_TotalActiveDirectionalLights{0};
    int m_TotalActivePointLights{0};
    int m_TotalActiveSpotLights{0};
};
