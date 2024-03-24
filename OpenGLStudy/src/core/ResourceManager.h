#pragma once
#include <memory>
#include <string>
#include <unordered_map>

class Mesh;
class Shader;
class ResourceManager
{
public:

    static std::string DEFAULT_MATERIAL_NAME;
    static std::string DEFAULT_MESH_CUBE_NAME;
    
    static void LoadDefaultResources();

    static std::shared_ptr<Shader> LoadShader(const std::string& vertexShaderPath, const std::string& fragShaderPath, const std::string& name);
    static std::shared_ptr<Shader> LoadShader(const std::string& singleFileShaderPath, const std::string& name);
    static std::shared_ptr<Shader> GetShader(const std::string& name);

    //TODO: implement loading custom mesh/model
    static std::shared_ptr<Mesh> GetMesh(const std::string& name);
    
    static void UnloadAll();

private:

    static std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
    static std::unordered_map<std::string, std::shared_ptr<Mesh>> m_Meshes;

    ResourceManager() = default;
};