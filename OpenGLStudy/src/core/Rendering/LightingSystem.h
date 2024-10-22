#pragma once
#include <memory>
#include <vector>

#include "FrameBuffer.h"
#include "Resolution.h"
#include "UniformBuffer.h"
#include "glm/vec3.hpp"

class SpotLightComponent;
class PointLightComponent;
class CameraComponent;
class Shader;
class DirectionalLightComponent;

namespace Rendering
{
    static constexpr int MAX_DIRECTIONAL_LIGHTS = 3;
    static constexpr int MAX_POINT_LIGHTS = 20;
    static constexpr int MAX_SPOT_LIGHTS = 20;
    
    struct AmbientLightShaderData
    {
        glm::vec3 Color{0.f};
        float PADDING_01{0.f};
    };

    struct DirectionalLightShaderData
    {
        float Intensity{0.f};
        glm::vec3 PADDING_01{0.f};

        glm::vec3 Direction{0.f};
        float PADDING_02{0.f};

        glm::vec3 Diffuse{0.f};
        float PADDING_03{0.f};

        glm::vec3 Specular{0.f};
        float PADDING_04{0.f};
    };

    struct PointLightShaderData
    {
        glm::vec3 Position{0.f};
        float Constant{1.f};
        float Linear{1.f};
        float Quadratic{1.f};
        
        float PADDING_01{0.f};
        float PADDING_02{0.f};

        glm::vec3 Diffuse{0.f};
        float PADDING_03{0.f};

        glm::vec3 Specular{0.f};
        float Intensity{0.f};
    };

    struct SpotLightShaderData
    {
        glm::vec3 Position{0.f};
        float PADDING_01{0.f};

        glm::vec3 Direction{0.f};

        float Cutoff{1.f};
        float OuterCutoff{1.f};
        float Constant{1.f};
        float Linear{1.f};
        float Quadratic{1.f};
        
        glm::vec3 Diffuse{0.f};
        float PADDING_02{0.f};
        
        glm::vec3 Specular{0.f};
        float Intensity{0.f};
    };

    struct LightingGeneralShaderData
    {
        glm::vec3 ViewPosition{0.f};
        float PADDING_01{0.f};

        AmbientLightShaderData AmbientLight{};
        int TotalDirectionalLights{0};
        int TotalPointLights{0};
        int TotalSpotLights{0};
    };
}

class LightingSystem
{
public:

    LightingSystem();
    void Setup();
    void Shutdown();
    
    void AddDirectionalLight(const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent);
    void AddPointLight(const std::shared_ptr<PointLightComponent>& pointLightComponent);
    void AddSpotLight(const std::shared_ptr<SpotLightComponent>& spotLightComponent);
    void SetupUniformsFor(Shader& shader) const;
    void UpdateLightingUniformBuffer(const CameraComponent& activeCamera);

    void SetAmbientLightColor(const glm::vec3& ambientLightColor) { m_AmbientLightColor = ambientLightColor; }
    glm::vec3 GetAmbientLightColor() const { return m_AmbientLightColor; }
    Framebuffer& GetShadowMapFramebuffer() const { return *m_ShadowDepthMapBuffer; }
    Rendering::Resolution GetShadowResolution() const { return m_ShadowDepthMapBuffer->GetResolution(); }
    std::shared_ptr<DirectionalLightComponent> GetMainDirectionalLight();

private:

    constexpr static int SHADOW_MAP_SLOT = 1;
    
    std::vector<std::shared_ptr<DirectionalLightComponent>> m_DirectionalLights{};
    std::vector<std::shared_ptr<PointLightComponent>> m_PointLights{};
    std::vector<std::shared_ptr<SpotLightComponent>> m_SpotLights{};
    glm::vec3 m_AmbientLightColor{0.05f, 0.15f, 0.175f};
    glm::vec3 m_DefaultSpecularColor{1.f};
    int m_TotalActiveDirectionalLights{0};
    int m_TotalActivePointLights{0};
    int m_TotalActiveSpotLights{0};

    std::unique_ptr<Rendering::UniformBuffer> m_GeneralUniformBuffer{};
    std::unique_ptr<Rendering::UniformBuffer> m_DirectionalUniformBuffer{};
    std::unique_ptr<Rendering::UniformBuffer> m_PointUniformBuffer{};
    std::unique_ptr<Rendering::UniformBuffer> m_SpotsUniformBuffer{};
    std::unique_ptr<Rendering::UniformBuffer> m_DirectionalMatrixUniformBuffer{};

    Rendering::LightingGeneralShaderData m_GeneralShaderData{};
    Rendering::DirectionalLightShaderData m_DirectionalsShaderData[Rendering::MAX_DIRECTIONAL_LIGHTS]{};
    Rendering::PointLightShaderData m_PointsShaderData[Rendering::MAX_POINT_LIGHTS];
    Rendering::SpotLightShaderData m_SpotsShaderData[Rendering::MAX_SPOT_LIGHTS];

    std::unique_ptr<Framebuffer> m_ShadowDepthMapBuffer{};

    void CreateUniformBuffers();
    void CreateShadowDepthMap(const Rendering::Resolution& resolution);
};


