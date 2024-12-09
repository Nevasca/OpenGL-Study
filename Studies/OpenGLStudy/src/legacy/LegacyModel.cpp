#include "LegacyModel.h"

#include <iostream>
#include <memory>
#include <assimp/postprocess.h>

#include "LegacyTexture.h"

LegacyModel::LegacyModel(const std::string& path)
{
    LoadModel(path);
}

void LegacyModel::Draw(LegacyShader& shader)
{
    for(const std::unique_ptr<LegacyMesh>& mesh : m_Meshes)
    {
        mesh->Draw(shader);
    }
}

// Note that some versions of Assimp tend to load models quite slow on debug mode
// try setting as release to proper testing
void LegacyModel::LoadModel(const std::string& path)
{
    Assimp::Importer importer;

    // path
    // pFlags: desired post-processing options
    //  aiProcess_Triangulate: if the model doesn't (entirely) consist of triangles, transform model primitives to triangles first
    //  aiProcess_FlipUVs: image is flipped on OpenGL, so we need to flip UV
    // Mode post=processing options: https://assimp.sourceforge.net/lib_html/postprocess_8h.html
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP: " << importer.GetErrorString() << "\n";
        return;
    }

    m_Directory = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
}

// Recursively process node and children nodes
void LegacyModel::ProcessNode(aiNode* node, const aiScene* scene)
{
    // Process all the node's meshes if any
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // Nodes contains just an index of the mesh, scene is the one that holds the meshes data
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.emplace_back(ProcessMesh(mesh, scene));
    }

    // Then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }

    // If we just iterated on scene->mMeshes we would lose the possibility of setting a parent children relationship
    // on meshes, where children move together with parent, for instance
    // this example doesn't use this, but it's nice to keep this approach for that
}

std::unique_ptr<LegacyMesh> LegacyModel::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<LegacyVertex> vertices{};
    std::vector<unsigned int> indices{};
    std::vector<std::shared_ptr<LegacyTexture>> textures{};

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        // Process vertex positions, normals and texture coordinates
        LegacyVertex vertex{};
        
        glm::vec3 position{};
        position.x = mesh->mVertices[i].x;
        position.y = mesh->mVertices[i].y;
        position.z = mesh->mVertices[i].z;
        vertex.Position = position;

        glm::vec3 normal{};
        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;
        vertex.Normal = normal;

        // Assimp allows model to have up to 8 texture coords per vertex, we are using just first one here
        if(mesh->mTextureCoords[0])
        {
            glm::vec2 uv{};
            uv.x = mesh->mTextureCoords[0][i].x;
            uv.y = mesh->mTextureCoords[0][i].y;
            vertex.UV = uv;
        }
        else
        {
            vertex.UV = glm::vec2{0.f, 0.f};
        }

        vertices.emplace_back(vertex);
    }

    // Process indices
    // Each face represents a single primitive. Since we used the post-processing for triangulating
    // they will all be triangles
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.emplace_back(face.mIndices[j]);
        }
    }

    // Process material
    if(mesh->mMaterialIndex > 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<std::shared_ptr<LegacyTexture>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<std::shared_ptr<LegacyTexture>> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return std::make_unique<LegacyMesh>(vertices, indices, textures);
}

std::vector<std::shared_ptr<LegacyTexture>> LegacyModel::LoadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName)
{
    std::vector<std::shared_ptr<LegacyTexture>> textures;
    
    for(unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString str;
        material->GetTexture(type, i, &str);

        bool skipLoading = false;
        for(unsigned j = 0; j < m_LoadedTextures.size(); j++)
        {
            // Already loaded, don't create again
            if(std::strcmp(m_LoadedTextures[j]->GetPath().data(), str.C_Str()) == 0)
            {
                textures.push_back(m_LoadedTextures[j]);
                skipLoading = true;
                
                break;
            }
        }

        if(skipLoading)
        {
            continue;
        }

        // Assuming textures are stored in same folder of the model
        std::string texturePath = m_Directory + "/" + str.C_Str();
        
        std::shared_ptr<LegacyTexture> texture = std::make_shared<LegacyTexture>(texturePath, true, true);
        texture->SetType(typeName);
        texture->SetPath(str.C_Str());

        textures.push_back(texture);
        m_LoadedTextures.push_back(texture);
    }

    return textures;
}
