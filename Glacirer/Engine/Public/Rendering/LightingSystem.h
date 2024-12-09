#pragma once
#include <memory>
#include <vector>

#include "FrameBuffer.h"
#include "Resolution.h"
#include "UniformBuffer.h"
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace Glacirer
{
    class SpotLightComponent;
    class PointLightComponent;
    class CameraComponent;
    class DirectionalLightComponent;

    namespace Rendering
    {
        class Shader;

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
            float Bias{0.005f};
            float NormalBias{0.001f};
            int CastShadow{1};

            glm::vec3 Direction{0.f};
            float PADDING_01{0.f};

            glm::vec3 Diffuse{0.f};
            float PADDING_02{0.f};

            glm::vec3 Specular{0.f};
            float PADDING_03{0.f};
        };

        struct PointLightShaderData
        {
            glm::vec3 Position{0.f};
            float Constant{1.f};
            float Linear{1.f};
            float Quadratic{1.f};
        
            int CastShadow{1};
            float PADDING_01{0.f};

            glm::vec3 Diffuse{0.f};
            float PADDING_02{0.f};

            glm::vec3 Specular{0.f};
            float Intensity{0.f};
        };

        struct SpotLightShaderData
        {
            glm::vec3 Position{0.f};
            int CastShadow{1};

            glm::vec3 Direction{0.f};

            float Cutoff{1.f};
            float OuterCutoff{1.f};
            float Constant{1.f};
            float Linear{1.f};
            float Quadratic{1.f};
        
            glm::vec3 Diffuse{0.f};
            float PADDING_01{0.f};
        
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

        struct DirectionalLightShadowMapShaderData
        {
            glm::mat4 ViewProjectionMatrix{};
        };

        struct PointLightShadowMapShaderData
        {
            glm::mat4 ViewProjectionMatrices[6]{};
        };

        struct SpotLightShadowMapShaderData
        {
            glm::mat4 ViewProjectionMatrix{};
        };

        class LightingSystem
        {
        public:

            LightingSystem();
            void Shutdown();
        
            void AddDirectionalLight(const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent);
            void AddPointLight(const std::shared_ptr<PointLightComponent>& pointLightComponent);
            void AddSpotLight(const std::shared_ptr<SpotLightComponent>& spotLightComponent);
            void SetupUniformsFor(Shader& shader) const;
            void UpdateLightingUniformBuffer(const CameraComponent& activeCamera);

            void SetAmbientLightColor(const glm::vec3& ambientLightColor) { m_AmbientLightColor = ambientLightColor; }
            glm::vec3 GetAmbientLightColor() const { return m_AmbientLightColor; }

            int GetTotalActiveDirectionalLights() const { return m_TotalActiveDirectionalLights; }
            Framebuffer& GetDirectionalShadowMapFramebuffer(const int activeLightIndex) const;
            const DirectionalLightComponent& GetDirectionalLight(const int activeLightIndex) const;
            int GetTotalActivePointLights() const { return m_TotalActivePointLights; }
            const Framebuffer& GetPointLightShadowMapFramebuffer(const int activeLightIndex) const;
            const PointLightComponent& GetPointLight(const int activeLightIndex) const;
            int GetTotalActiveSpotLights() const { return m_TotalActiveSpotLights; }
            const Framebuffer& GetSpotLightShadowMapFramebuffer(const int activeLightIndex) const;
            const SpotLightComponent& GetSpotLight(const int activeLightIndex) const;

        private:

            constexpr static int DIRECTIONAL_SHADOW_MAP_START_SLOT = 1;
            constexpr static int POINT_SHADOW_MAP_START_SLOT = DIRECTIONAL_SHADOW_MAP_START_SLOT + Rendering::MAX_DIRECTIONAL_LIGHTS;
            constexpr static int SPOT_SHADOW_MAP_START_SLOT = POINT_SHADOW_MAP_START_SLOT + Rendering::MAX_POINT_LIGHTS;
        
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
            std::unique_ptr<Rendering::UniformBuffer> m_PointLightMatricesUniformBuffer{};
            std::unique_ptr<Rendering::UniformBuffer> m_SpotLightMatricesUniformBuffer{};

            Rendering::LightingGeneralShaderData m_GeneralShaderData{};
            Rendering::DirectionalLightShaderData m_DirectionalsShaderData[Rendering::MAX_DIRECTIONAL_LIGHTS]{};
            Rendering::PointLightShaderData m_PointsShaderData[Rendering::MAX_POINT_LIGHTS]{};
            Rendering::SpotLightShaderData m_SpotsShaderData[Rendering::MAX_SPOT_LIGHTS]{};
            Rendering::DirectionalLightShadowMapShaderData m_DirectionalLightShadowMapShaderData[Rendering::MAX_DIRECTIONAL_LIGHTS]{};
            Rendering::PointLightShadowMapShaderData m_PointLightShadowMapShaderData[Rendering::MAX_POINT_LIGHTS]{};
            Rendering::SpotLightShadowMapShaderData m_SpotLightShadowMapShaderData[Rendering::MAX_SPOT_LIGHTS]{};

            // TODO: fix directional shadow map quality to not required separate and higher resolution
            Rendering::Resolution m_DirectionalShadowResolution{2048, 2048};
            Rendering::Resolution m_ShadowResolution{1024, 1024};
            std::vector<std::unique_ptr<Framebuffer>> m_DirectionalShadowMapBuffers{};
            std::vector<std::unique_ptr<Framebuffer>> m_PointLightShadowMapBuffers{};
            std::vector<std::unique_ptr<Framebuffer>> m_SpotLightShadowMapBuffers{};

            void UpdateDirectionalShadowMapUniformBuffers();
            void UpdatePointShadowMapUniformBuffers();
            void UpdateSpotShadowMapUniformBuffers();

            void CreateUniformBuffers();
            void BindShadowMapTextures();
            void UnbindShadowMapTextures();
            void CreateShadowMaps();
            void CreateDirectionalLightShadowMapFor(int lightIndex);
            void CreatePointLightShadowMapFor(int lightIndex);
            void CreateSportLightShadowMapFor(int lightIndex);
        };
    }
}
