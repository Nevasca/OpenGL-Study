#pragma once
#include <map>
#include <memory>
#include <vector>

#include "MeshRenderer.h"
#include "InstancedArray.h"
#include "LightingSystem.h"
#include "PostProcessingSystem.h"
#include "core/Basics/Components/DirectionalLightComponent.h"
#include "FrameBuffer.h"

class PostProcessingComponent;
class SpotLightComponent;
class PointLightComponent;
class MeshComponent;
class Shader;
class CameraComponent;

struct ActiveShader
{
    std::shared_ptr<Shader> Shader{};
    unsigned int UsageCount{0};
};

class RenderSystem
{
public:

    RenderSystem();
    void Shutdown();

    void AddMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent);
    void RemoveMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent);
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

private:

    constexpr static int MAX_INSTANCED_AMOUNT_PER_CALL = 10000;

    MeshRenderer m_MeshRenderer{};
    LightingSystem m_LightingSystem{};
    PostProcessingSystem m_PostProcessingSystem{};

    std::map<unsigned int, std::map<unsigned int, std::vector<std::shared_ptr<MeshComponent>>>> m_MeshComponents{}; // keyed by VAO and material ID
    std::map<unsigned int, ActiveShader> m_UniqueActiveShaders{}; // Keyed by shader id
    std::shared_ptr<Shader> m_WorldOverrideShader{}; // if set, render world using only this shader

    std::unique_ptr<InstancedArray> m_InstancedArray{};
    std::unique_ptr<Framebuffer> m_Framebuffer{};

    void UpdateGlobalShaderUniforms(const CameraComponent& activeCamera) const;
    void RenderWorldObjects();
    void CreateInstancedBuffer();
    void SetupInstancedMesh(const Mesh& mesh);
};
