#include "ResourceManager.h"

#include <memory>

#include "Rendering/Cubemap.h"
#include "Rendering/Material.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Resources/MeshResource.h"
#include "Resources/ShaderResource.h"
#include "Resources/TextureResource.h"

std::unordered_map<std::string, std::shared_ptr<Shader>> ResourceManager::m_Shaders{};
std::unordered_map<std::string, std::shared_ptr<Mesh>> ResourceManager::m_Meshes{};
std::unordered_map<std::string, std::shared_ptr<Material>> ResourceManager::m_Materials{};
std::unordered_map<std::string, std::shared_ptr<Texture>> ResourceManager::m_Textures{};
std::unordered_map<std::string, std::shared_ptr<ModelData>> ResourceManager::m_Models{};
std::unordered_map<std::string, std::shared_ptr<Rendering::Cubemap>> ResourceManager::m_Cubemaps{};

unsigned int ResourceManager::m_LastMaterialID = 0;
std::string ResourceManager::DEFAULT_SHADER_NAME = "Default";
std::string ResourceManager::DEFAULT_MATERIAL_NAME = "Default";
std::string ResourceManager::DEFAULT_MESH_CUBE_NAME = "Cube";
std::string ResourceManager::DEFAULT_MESH_QUAD_NAME = "Quad";
std::string ResourceManager::DEFAULT_MESH_SPHERE_NAME = "Sphere";

void ResourceManager::LoadDefaultResources()
{
    std::shared_ptr<Shader> defaultShader = LoadShader("res/core/shaders/BlinnPhong.glsl", DEFAULT_SHADER_NAME);

    std::shared_ptr<Material> defaultMaterial = CreateMaterial(DEFAULT_MATERIAL_NAME);
    defaultMaterial->SetColor("u_Color", glm::vec4(0.5f, 0.5f, 0.5f, 1.f));
    defaultMaterial->SetFloat("u_ReflectionValue", 0.f);

    m_Meshes[DEFAULT_MESH_CUBE_NAME] = MeshResource::LoadCube();
    m_Meshes[DEFAULT_MESH_QUAD_NAME] = MeshResource::LoadQuad();
    m_Meshes[DEFAULT_MESH_SPHERE_NAME] = MeshResource::LoadSphere();
}

std::shared_ptr<Material> ResourceManager::GetDefaultMaterial()
{
    return m_Materials[DEFAULT_MATERIAL_NAME];
}

std::shared_ptr<Shader> ResourceManager::GetDefaultShader()
{
    return m_Shaders[DEFAULT_SHADER_NAME];
}

std::shared_ptr<Mesh> ResourceManager::GetDefaultCube()
{
    return m_Meshes[DEFAULT_MESH_CUBE_NAME];
}

std::shared_ptr<Mesh> ResourceManager::GetDefaultQuad()
{
    return m_Meshes[DEFAULT_MESH_QUAD_NAME];
}

std::shared_ptr<Mesh> ResourceManager::GetDefaultSphere()
{
    return m_Meshes[DEFAULT_MESH_SPHERE_NAME];
}

std::shared_ptr<Shader> ResourceManager::LoadShader(const std::string& vertexShaderPath, const std::string& fragShaderPath, const std::string& name)
{
    m_Shaders[name] = ShaderResource::LoadShaderFromFile(vertexShaderPath, fragShaderPath);
    
    return m_Shaders[name]; 
}

std::shared_ptr<Shader> ResourceManager::LoadShader(const std::string& singleFileShaderPath, const std::string& name)
{
    const std::shared_ptr<Shader> shader = ShaderResource::LoadShaderFromFile(singleFileShaderPath);
    shader->SetName(name);
    m_Shaders[name] = shader;

    return shader;
}

std::shared_ptr<Shader> ResourceManager::GetOrLoadShader(const std::string& singleFileShaderPath, const std::string& name)
{
    if(m_Shaders.find(name) == m_Shaders.end())
    {
        return LoadShader(singleFileShaderPath, name);
    }

    return m_Shaders[name]; 
}

std::shared_ptr<Shader> ResourceManager::GetShader(const std::string& name)
{
    return m_Shaders[name];
}

std::shared_ptr<Material> ResourceManager::CreateMaterial(const std::string& name)
{
    std::shared_ptr<Material> material = CreateMaterial(name, DEFAULT_SHADER_NAME);
    material->SetInt("u_MaterialShininess", 64);

    return material;
}

std::shared_ptr<Material> ResourceManager::CreateMaterial(const std::string& name, const std::string& shaderName)
{
    std::shared_ptr<Material> material = std::make_shared<Material>();
    material->SetId(m_LastMaterialID++);
    material->SetShader(m_Shaders[shaderName]);
    material->SetName(name);

    m_Materials[name] = material;

    return material;
}

std::shared_ptr<Material> ResourceManager::GetOrCreateMaterial(const std::string& name, const std::string& shaderName)
{
    if(m_Materials.find(name) == m_Materials.end())
    {
        return CreateMaterial(name, shaderName);
    }

    return m_Materials[name];
}

std::shared_ptr<Material> ResourceManager::GetMaterial(const std::string& name)
{
    return m_Materials[name];
}

const std::unordered_map<std::string, std::shared_ptr<Material>>& ResourceManager::GetAllMaterials()
{
    return m_Materials;
}

std::shared_ptr<Texture> ResourceManager::LoadTexture(const std::string& filePath, const std::string& name, const TextureSettings& settings, bool bFlipVertically)
{
    std::shared_ptr<Texture> texture = TextureResource::LoadTextureFromFile(filePath, settings, bFlipVertically);
    texture->SetName(name);
    
    m_Textures[name] = texture; 

    return texture;
}

std::shared_ptr<Texture> ResourceManager::GetTexture(const std::string& name)
{
    return m_Textures[name];
}

std::shared_ptr<Rendering::Cubemap> ResourceManager::LoadCubemap(const Rendering::CubemapLoadSettings& loadSettings, const std::string& name)
{
    std::shared_ptr<Rendering::Cubemap> cubemap = TextureResource::LoadCubemapFromFile(loadSettings);
    cubemap->SetName(name);

    m_Cubemaps[name] = cubemap;

    return cubemap;
}

std::shared_ptr<Rendering::Cubemap> ResourceManager::GetOrLoadCubemap(const Rendering::CubemapLoadSettings& loadSettings, const std::string& name)
{
    if(m_Cubemaps.find(name) == m_Cubemaps.end())
    {
        return LoadCubemap(loadSettings, name);
    }

    return m_Cubemaps[name];
}

std::shared_ptr<Rendering::Cubemap> ResourceManager::GetCubemap(const std::string& name)
{
    return m_Cubemaps[name];
}

std::shared_ptr<Mesh> ResourceManager::GetMesh(const std::string& name)
{
    return m_Meshes[name];
}

std::shared_ptr<ModelData> ResourceManager::LoadModel(const std::string& filePath, const std::string& name)
{
    m_Models[name] = MeshResource::LoadModelFromFile(filePath);

    return m_Models[name];
}

std::shared_ptr<ModelData> ResourceManager::GetModel(const std::string& name)
{
    return m_Models[name];
}

void ResourceManager::UnloadAll()
{
    m_Shaders.clear();
    m_Meshes.clear();
    m_Materials.clear();
    m_Textures.clear();
    m_Models.clear();
    m_Cubemaps.clear();

    m_LastMaterialID = 0;
}
