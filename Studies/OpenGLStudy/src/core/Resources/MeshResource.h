#pragma once
#include <memory>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class ModelData;
class Mesh;

class MeshResource
{
public:

    static std::shared_ptr<Mesh> LoadCube();
    static std::shared_ptr<Mesh> LoadQuad();
    static std::shared_ptr<Mesh> LoadSphere();
    static std::shared_ptr<ModelData> LoadModelFromFile(const std::string& filePath);

private:

    static void ProcessNode(aiNode* node, const aiScene* scene, ModelData& outModel);
    static std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
};
