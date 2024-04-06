#pragma once
#include <map>
#include <memory>
#include <vector>

#include "MeshRenderer.h"
#include "InstancedArray.h"
#include "LightingSystem.h"
#include "core/Basics/Components/DirectionalLightComponent.h"

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
    void Render(const CameraComponent& activeCamera);

private:

    constexpr static int MAX_INSTANCED_AMOUNT_PER_CALL = 10000;

    MeshRenderer m_MeshRenderer{};
    LightingSystem m_LightingSystem{};

    std::map<unsigned int, std::map<unsigned int, std::vector<std::shared_ptr<MeshComponent>>>> m_MeshComponents{}; // keyed by VAO and material ID
    std::map<unsigned int, ActiveShader> m_UniqueActiveShaders{}; // Keyed by shader id

    std::unique_ptr<InstancedArray> m_InstancedArray{};

    void UpdateGlobalShaderUniforms(const CameraComponent& activeCamera) const;
    void CreateInstancedBuffer();
    void SetupInstancedMesh(const Mesh& mesh);
};
