#pragma once
#include <map>
#include <memory>
#include <vector>

#include "MeshRenderer.h"

class MeshComponent;
class Shader;
class CameraComponent;

class RenderSystem
{
public:

    RenderSystem();
    void Shutdown();

    void AddMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent);
    void Render(const CameraComponent& activeCamera);

private:

    constexpr static int MAX_INSTANCED_AMOUNT_PER_CALL = 10000;

    MeshRenderer m_MeshRenderer{};

    std::map<unsigned int, std::vector<std::shared_ptr<MeshComponent>>> m_MeshComponents{}; // keyed by VAO
    //TODO: after implementing materials, we will need to change key for material id
    std::map<unsigned int, std::shared_ptr<Shader>> m_UniqueActiveShaders{}; // keyed by shader id TODO: organize by material and mesh

    unsigned int m_InstancedBufferId{0};

    void UpdateGlobalShaderUniforms(const CameraComponent& activeCamera) const;
    void CreateInstancedBuffer();
    void SetupInstancedMesh(const Mesh& mesh);
};
