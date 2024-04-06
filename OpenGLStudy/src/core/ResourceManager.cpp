#include "ResourceManager.h"

#include <memory>

#include "Rendering/Material.h"
#include "Resources/MeshResource.h"
#include "Resources/ShaderResource.h"
#include "Resources/TextureResource.h"

std::unordered_map<std::string, std::shared_ptr<Shader>> ResourceManager::m_Shaders{};
std::unordered_map<std::string, std::shared_ptr<Mesh>> ResourceManager::m_Meshes{};
std::unordered_map<std::string, std::shared_ptr<Material>> ResourceManager::m_Materials{};
std::unordered_map<std::string, std::shared_ptr<Texture>> ResourceManager::m_Textures{};

unsigned int ResourceManager::m_LastMaterialID = 0;
std::string ResourceManager::DEFAULT_SHADER_NAME = "Default";
std::string ResourceManager::DEFAULT_MATERIAL_NAME = "Default";
std::string ResourceManager::DEFAULT_MESH_CUBE_NAME = "Cube";

void ResourceManager::LoadDefaultResources()
{
    std::shared_ptr<Shader> defaultShader = LoadShader("res/core/shaders/Default.glsl", DEFAULT_SHADER_NAME);

    std::shared_ptr<Material> defaultMaterial = LoadMaterial(DEFAULT_MATERIAL_NAME);
    defaultMaterial->SetShader(defaultShader);
    defaultMaterial->SetColor("u_Color", glm::vec4(0.5f, 0.5f, 0.5f, 1.f));

    m_Meshes[DEFAULT_MESH_CUBE_NAME] = MeshResource::LoadCube();
}

std::shared_ptr<Shader> ResourceManager::LoadShader(const std::string& vertexShaderPath, const std::string& fragShaderPath, const std::string& name)
{
    m_Shaders[name] = ShaderResource::LoadShaderFromFile(vertexShaderPath, fragShaderPath);
    
    return m_Shaders[name]; 
}

std::shared_ptr<Shader> ResourceManager::LoadShader(const std::string& singleFileShaderPath, const std::string& name)
{
    m_Shaders[name] = ShaderResource::LoadShaderFromFile(singleFileShaderPath);

    return m_Shaders[name];
}

std::shared_ptr<Shader> ResourceManager::GetShader(const std::string& name)
{
    return m_Shaders[name];
}

std::shared_ptr<Material> ResourceManager::CreateMaterial(const std::string& name, const std::string& shaderName)
{
    std::shared_ptr<Material> material = std::make_shared<Material>();
    material->SetId(m_LastMaterialID++);
    material->SetShader(m_Shaders[shaderName]);

    m_Materials[name] = material;

    return m_Materials[name];
}

std::shared_ptr<Material> ResourceManager::LoadMaterial(const std::string& name)
{
    // TODO: implement loading from file
    m_Materials[name] = std::make_shared<Material>();
    m_Materials[name]->SetId(m_LastMaterialID++);

    return m_Materials[name];
}

std::shared_ptr<Material> ResourceManager::GetMaterial(const std::string& name)
{
    return m_Materials[name];
}

std::shared_ptr<Texture> ResourceManager::LoadTexture(const std::string& filePath, const std::string& name, bool bUseAlpha, bool bFlipVertically)
{
    m_Textures[name] = TextureResource::LoadTextureFromFile(filePath, bUseAlpha, bFlipVertically);

    return m_Textures[name];
}

std::shared_ptr<Texture> ResourceManager::GetTexture(const std::string& name)
{
    return m_Textures[name];
}

std::shared_ptr<Mesh> ResourceManager::GetMesh(const std::string& name)
{
    return m_Meshes[name];
}

void ResourceManager::UnloadAll()
{
    m_Shaders.clear();
    m_Meshes.clear();
    m_Materials.clear();
    m_Textures.clear();

    m_LastMaterialID = 0;
}
