#pragma once
#include <memory>
#include <vector>

class Mesh;

class ModelData
{
public:

    void AddMesh(const std::shared_ptr<Mesh>&& mesh) { m_Meshes.emplace_back(mesh); }
    const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const { return m_Meshes; }

private:

    std::vector<std::shared_ptr<Mesh>> m_Meshes{};
};
