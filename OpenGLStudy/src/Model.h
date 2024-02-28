#pragma once
#include <string>
#include <vector>

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>


class Shader;

class Model
{
public:

    Model(const std::string& path);

    void Draw(Shader& shader);

private:

    std::vector<std::unique_ptr<Mesh>> m_Meshes{};
    std::string m_Directory; // Used for loading textures
    std::vector<std::shared_ptr<Texture>> m_LoadedTextures{};

    void LoadModel(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    std::unique_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName);
};
