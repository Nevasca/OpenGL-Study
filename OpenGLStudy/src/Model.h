#pragma once
#include <string>
#include <vector>

#include "LegacyMesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>


class LegacyShader;

class Model
{
public:

    Model(const std::string& path);

    void Draw(LegacyShader& shader);

private:

    std::vector<std::unique_ptr<LegacyMesh>> m_Meshes{};
    std::string m_Directory; // Used for loading textures
    std::vector<std::shared_ptr<LegacyTexture>> m_LoadedTextures{};

    void LoadModel(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    std::unique_ptr<LegacyMesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<std::shared_ptr<LegacyTexture>> LoadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName);
};
