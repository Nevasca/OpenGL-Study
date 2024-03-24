#include "ResourceManager.h"

#include "Resources/MeshResource.h"
#include "Resources/ShaderResource.h"

std::unordered_map<std::string, std::shared_ptr<Shader>> ResourceManager::m_Shaders{};
std::unordered_map<std::string, std::shared_ptr<Mesh>> ResourceManager::m_Meshes{};
std::string ResourceManager::DEFAULT_MATERIAL_NAME = "Default";
std::string ResourceManager::DEFAULT_MESH_CUBE_NAME = "Cube";

void ResourceManager::LoadDefaultResources()
{
    LoadShader("res/core/shaders/Default.glsl", DEFAULT_MATERIAL_NAME);

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

    return nullptr;
}

std::shared_ptr<Shader> ResourceManager::GetShader(const std::string& name)
{
    return m_Shaders[name];
}

std::shared_ptr<Mesh> ResourceManager::GetMesh(const std::string& name)
{
    return m_Meshes[name];
}

void ResourceManager::UnloadAll()
{
    m_Shaders.clear();
    m_Meshes.clear();
}
