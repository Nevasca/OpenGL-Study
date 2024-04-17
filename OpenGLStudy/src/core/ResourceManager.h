#pragma once
#include <memory>
#include <string>
#include <unordered_map>

class ModelData;
class Texture;
class Material;
class Mesh;
class Shader;
class ResourceManager
{
public:

    static std::string DEFAULT_SHADER_NAME;
    static std::string DEFAULT_MATERIAL_NAME;
    static std::string DEFAULT_MESH_CUBE_NAME;
    static std::string DEFAULT_MESH_QUAD_NAME;
    static std::string DEFAULT_MESH_SPHERE_NAME;
    
    static void LoadDefaultResources();

    static std::shared_ptr<Shader> LoadShader(const std::string& vertexShaderPath, const std::string& fragShaderPath, const std::string& name);
    static std::shared_ptr<Shader> LoadShader(const std::string& singleFileShaderPath, const std::string& name);
    static std::shared_ptr<Shader> GetShader(const std::string& name);
    static std::shared_ptr<Material> CreateMaterial(const std::string& name, const std::string& shaderName);
    static std::shared_ptr<Material> GetMaterial(const std::string& name);
    static const std::unordered_map<std::string, std::shared_ptr<Material>>& GetAllMaterials();
    static std::shared_ptr<Texture> LoadTexture(const std::string& filePath, const std::string& name, bool bUseAlpha, bool bFlipVertically = false);
    static std::shared_ptr<Texture> GetTexture(const std::string& name);

    //TODO: implement loading custom mesh/model
    static std::shared_ptr<Mesh> GetMesh(const std::string& name);
    static std::shared_ptr<ModelData> LoadModel(const std::string& filePath, const std::string& name);
    static std::shared_ptr<ModelData> GetModel(const std::string& name);
    
    static void UnloadAll();

private:

    static std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
    static std::unordered_map<std::string, std::shared_ptr<Mesh>> m_Meshes;
    static std::unordered_map<std::string, std::shared_ptr<Material>> m_Materials;
    static std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
    static std::unordered_map<std::string, std::shared_ptr<ModelData>> m_Models;
    
    static unsigned int m_LastMaterialID;

    ResourceManager() = default;
};
