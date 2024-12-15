#pragma once
#include <memory>

#include "EngineAPI.h"
#include "Device.h"
#include "MeshRenderer.h"
#include "InstancedArray.h"
#include "LightingSystem.h"
#include "PostProcessingSystem.h"
#include "Basics/Components/DirectionalLightComponent.h"
#include "FrameBuffer.h"
#include "MeshComponentRenderSet.h"
#include "ShaderRenderSet.h"
#include "UniformBuffer.h"

namespace Glacirer
{
    class SkyboxComponent;
    class PostProcessingComponent;
    class SpotLightComponent;
    class PointLightComponent;
    class MeshComponent;
    class CameraComponent;

    namespace Rendering
    {
        class Cubemap;
        class Shader;

        class ENGINE_API RenderSystem
        {
        public:

            RenderSystem(const unsigned int totalMSAASamples);
            void Setup();
            void Shutdown();

            void AddMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent);
            void RemoveMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent);
            void AddOutlinedMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent);
            void RemoveOutlinedMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent);
            void AddDirectionalLight(const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent);
            void RemoveDirectionalLight(const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent);
            void AddPointLight(const std::shared_ptr<PointLightComponent>& pointLightComponent);
            void RemovePointLight(const std::shared_ptr<PointLightComponent>& pointLightComponent);
            void AddSpotLight(const std::shared_ptr<SpotLightComponent>& spotLightComponent);
            void RemoveSpotLight(const std::shared_ptr<SpotLightComponent>& spotLightComponent);
            void SetPostProcessingComponent(const std::shared_ptr<PostProcessingComponent>& postProcessingComponent);
            void RemovePostProcessingComponent(const std::shared_ptr<PostProcessingComponent>& postProcessingComponent);
            void SetSkyboxComponent(const std::shared_ptr<SkyboxComponent>& skyboxComponent);
            void RemoveSkyboxComponent(const std::shared_ptr<SkyboxComponent>& skyboxComponent);
            void Render(const CameraComponent& activeCamera);

            void SetAmbientLightColor(const glm::vec3& ambientLightColor) { m_LightingSystem.SetAmbientLightColor(ambientLightColor); }
            glm::vec3 GetAmbientLightColor() const { return m_LightingSystem.GetAmbientLightColor(); }
            void SetClearColor(const glm::vec4& clearColor) const { m_MultisampleFramebuffer->SetClearColor(clearColor); }
            glm::vec4 GetClearColor() const { return m_MultisampleFramebuffer->GetClearColor(); }
            void SetOverrideShader(const std::shared_ptr<Shader>& overrideShader, bool bSetupUniforms = true);
            Rendering::Device& GetDevice() { return m_Device; }
            void ToggleSkybox(bool bEnable) { bIsSkyboxEnabled = bEnable; }

        private:

            constexpr static int MAX_INSTANCED_AMOUNT_PER_CALL = 10000;
            constexpr static int SKYBOX_CUBEMAP_SLOT = 0;

            MeshRenderer m_MeshRenderer{};
            LightingSystem m_LightingSystem{};
            PostProcessingSystem m_PostProcessingSystem{};
            Rendering::Device m_Device{};

            Rendering::MeshComponentRenderSet m_OpaqueMeshComponentSet{};
            Rendering::MeshComponentRenderSet m_TransparentMeshComponentSet{};
            Rendering::MeshComponentRenderSet m_OpaqueOutlinedMeshComponentSet{};
            Rendering::MeshComponentRenderSet m_TransparentOutlinedMeshComponentSet{};
            Rendering::ShaderRenderSet m_UniqueActiveShaderSet{};
            std::shared_ptr<Shader> m_WorldOverrideShader{}; // if set, render world using only this shader
            std::unique_ptr<Rendering::UniformBuffer> m_MatricesUniformBuffer{};
            std::unique_ptr<Rendering::UniformBuffer> m_CameraUniformBuffer{};

            std::unique_ptr<InstancedArray> m_InstancedArray{};
            std::unique_ptr<Framebuffer> m_MultisampleFramebuffer{};
            std::unique_ptr<Framebuffer> m_IntermediateFramebuffer{};
    
            std::shared_ptr<Shader> m_OutlineShader{};
            std::shared_ptr<SkyboxComponent> m_SkyboxComponent{};
            bool bIsSkyboxEnabled{true};

            std::shared_ptr<Shader> m_DirectionalDepthShader{};
            std::shared_ptr<Shader> m_OmnidirectionalDepthShader{};

            Rendering::MeshComponentRenderSet& GetComponentRenderSetFor(const std::shared_ptr<MeshComponent>& meshComponent);
            Rendering::MeshComponentRenderSet& GetOutlinedComponentRenderSetFor(const std::shared_ptr<MeshComponent>& meshComponent);
            void UpdateGlobalShaderUniforms(const CameraComponent& activeCamera);
            void UpdateCameraMatricesShaderUniforms(const CameraComponent& activeCamera);
            void RenderObjects(const Rendering::MeshComponentRenderSet& meshComponentSet);
            void RenderObjectsSortedByDistance(const Rendering::MeshComponentRenderSet& meshComponentSet, const glm::vec3& cameraPosition);
            void RenderSkybox(const CameraComponent& activeCamera);
            void RenderWorld(const CameraComponent& activeCamera);
            void RenderShadowPass(const CameraComponent& activeCamera);
            void RenderDirectionalShadowPass();
            void RenderPointShadowPass();
            void RenderSpotShadowPass();
            void RenderWorldForShadowPass(const glm::vec3& lightPosition);
            void RenderOutlinedObjects(const CameraComponent& activeCamera);
            void CreateInstancedBuffer();
            void CreateUniformBuffers();
            void SetupUniformsFor(Shader& shader) const;
            void SetupOutlineRendering();
            bool IsSkyboxActive() const;
            void SetupShadowRendering();
        };
    }
}
