#pragma once
#include <memory>
#include <vector>

#include "FrameBuffer.h"
#include "Resolution.h"
#include "UniformBuffer.h"
#include "RenderingConstants.h"
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
            void RemoveDirectionalLight(const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent);
            void AddPointLight(const std::shared_ptr<PointLightComponent>& pointLightComponent);
            void RemovePointLight(const std::shared_ptr<PointLightComponent>& pointLightComponent);
            void AddSpotLight(const std::shared_ptr<SpotLightComponent>& spotLightComponent);
            void RemoveSpotLight(const std::shared_ptr<SpotLightComponent>& spotLightComponent);
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
            constexpr static int POINT_SHADOW_MAP_START_SLOT = DIRECTIONAL_SHADOW_MAP_START_SLOT + MAX_DIRECTIONAL_LIGHTS;
            constexpr static int SPOT_SHADOW_MAP_START_SLOT = POINT_SHADOW_MAP_START_SLOT + MAX_POINT_LIGHTS;
        
            std::vector<std::shared_ptr<DirectionalLightComponent>> m_DirectionalLights{};
            std::vector<std::shared_ptr<PointLightComponent>> m_PointLights{};
            std::vector<std::shared_ptr<SpotLightComponent>> m_SpotLights{};
            glm::vec3 m_AmbientLightColor{0.05f, 0.15f, 0.175f};
            glm::vec3 m_DefaultSpecularColor{1.f};
            int m_TotalActiveDirectionalLights{0};
            int m_TotalActivePointLights{0};
            int m_TotalActiveSpotLights{0};

            std::unique_ptr<UniformBuffer> m_GeneralUniformBuffer{};
            std::unique_ptr<UniformBuffer> m_DirectionalUniformBuffer{};
            std::unique_ptr<UniformBuffer> m_PointUniformBuffer{};
            std::unique_ptr<UniformBuffer> m_SpotsUniformBuffer{};
            std::unique_ptr<UniformBuffer> m_DirectionalMatrixUniformBuffer{};
            std::unique_ptr<UniformBuffer> m_PointLightMatricesUniformBuffer{};
            std::unique_ptr<UniformBuffer> m_SpotLightMatricesUniformBuffer{};

            LightingGeneralShaderData m_GeneralShaderData{};
            DirectionalLightShaderData m_DirectionalsShaderData[MAX_DIRECTIONAL_LIGHTS]{};
            PointLightShaderData m_PointsShaderData[MAX_POINT_LIGHTS]{};
            SpotLightShaderData m_SpotsShaderData[MAX_SPOT_LIGHTS]{};
            DirectionalLightShadowMapShaderData m_DirectionalLightShadowMapShaderData[MAX_DIRECTIONAL_LIGHTS]{};
            PointLightShadowMapShaderData m_PointLightShadowMapShaderData[MAX_POINT_LIGHTS]{};
            SpotLightShadowMapShaderData m_SpotLightShadowMapShaderData[MAX_SPOT_LIGHTS]{};

            // TODO: fix directional shadow map quality to not required separate and higher resolution
            Resolution m_DirectionalShadowResolution{2048, 2048};
            Resolution m_ShadowResolution{1024, 1024};
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
            void CreateDirectionalLightShadowMap();
            void RemoveDirectionalLightShadowMap();
            void CreatePointLightShadowMap();
            void RemovePointLightShadowMap();
            void CreateSpotLightShadowMap();
            void RemoveSpotLightShadowMap();
        };
    }
}
