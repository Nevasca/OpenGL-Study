#pragma once
#include <memory>

#include "Device.h"
#include "MeshRenderer.h"
#include "InstancedArray.h"
#include "LightingSystem.h"
#include "PostProcessingSystem.h"
#include "core/Basics/Components/DirectionalLightComponent.h"
#include "FrameBuffer.h"
#include "MeshComponentRenderSet.h"
#include "ShaderRenderSet.h"

class PostProcessingComponent;
class SpotLightComponent;
class PointLightComponent;
class MeshComponent;
class Shader;
class CameraComponent;

class RenderSystem
{
public:

    RenderSystem();
    void Shutdown();

    void AddMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent);
    void RemoveMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent);
    void AddOutlinedMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent);
    void RemoveOutlinedMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent);
    void AddDirectionalLight(const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent);
    void AddPointLight(const std::shared_ptr<PointLightComponent>& pointLightComponent);
    void AddSpotLight(const std::shared_ptr<SpotLightComponent>& spotLightComponent);
    void SetPostProcessingComponent(const std::shared_ptr<PostProcessingComponent>& postProcessingComponent);
    void RemovePostProcessingComponent(const std::shared_ptr<PostProcessingComponent>& postProcessingComponent);
    void Render(const CameraComponent& activeCamera);

    void SetAmbientLightColor(const glm::vec3& ambientLightColor) { m_LightingSystem.SetAmbientLightColor(ambientLightColor); }
    glm::vec3 GetAmbientLightColor() const { return m_LightingSystem.GetAmbientLightColor(); }
    void SetClearColor(const glm::vec4& clearColor) const { m_Framebuffer->SetClearColor(clearColor); }
    glm::vec4 GetClearColor() const { return m_Framebuffer->GetClearColor(); }
    void SetOverrideShader(const std::shared_ptr<Shader>& overrideShader) { m_WorldOverrideShader = overrideShader; }
    Rendering::Device& GetDevice() { return m_Device; }

private:

    constexpr static int MAX_INSTANCED_AMOUNT_PER_CALL = 10000;

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

    std::unique_ptr<InstancedArray> m_InstancedArray{};
    std::unique_ptr<Framebuffer> m_Framebuffer{};
    std::shared_ptr<Shader> m_OutlineShader{};

    Rendering::MeshComponentRenderSet& GetComponentRenderSetFor(const std::shared_ptr<MeshComponent>& meshComponent);
    Rendering::MeshComponentRenderSet& GetOutlinedComponentRenderSetFor(const std::shared_ptr<MeshComponent>& meshComponent);
    void UpdateGlobalShaderUniforms(const CameraComponent& activeCamera);
    void RenderObjects(const Rendering::MeshComponentRenderSet& meshComponentSet);
    void RenderObjectsSortedByDistance(const Rendering::MeshComponentRenderSet& meshComponentSet, const glm::vec3& cameraPosition);
    void RenderWorldObjects(const CameraComponent& activeCamera);
    void RenderOutlinedObjects(const CameraComponent& activeCamera);
    void CreateInstancedBuffer();
    void SetupOutlineRendering();
};
