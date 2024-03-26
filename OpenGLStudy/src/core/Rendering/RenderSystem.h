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

    void Shutdown();

    void AddMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent);
    void Render(CameraComponent& activeCamera);

private:

    MeshRenderer m_MeshRenderer{};

    //Keyed by shader id
    //TODO: after implementing materials, we will need to change key for material id
    std::map<unsigned int, std::vector<std::shared_ptr<MeshComponent>>> m_MeshComponents{};
    std::map<unsigned int, std::shared_ptr<Shader>> m_UniqueActiveShaders{};
};
