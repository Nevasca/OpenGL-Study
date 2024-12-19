#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "EngineAPI.h"
#include "Rendering/TextureSettings.h"

namespace Glacirer
{
    namespace Rendering
    {
        struct CubemapLoadSettings;
        class Cubemap;
        class ModelData;
        class Texture;
        class Material;
        class Mesh;
        class Shader;
    }

    namespace Resources
    {
        class ENGINE_API ResourceManager
        {
        public:

            static std::string RESOURCES_PATH;
    
            static void LoadDefaultResources();
            static std::shared_ptr<Rendering::Material> GetDefaultMaterial();
            static std::shared_ptr<Rendering::Shader> GetDefaultShader();
            static std::shared_ptr<Rendering::Mesh> GetDefaultCube();
            static std::shared_ptr<Rendering::Mesh> GetDefaultQuad();
            static std::shared_ptr<Rendering::Mesh> GetDefaultSphere();

            static std::shared_ptr<Rendering::Shader> LoadShader(const std::string& vertexShaderPath, const std::string& fragShaderPath, const std::string& name);
            static std::shared_ptr<Rendering::Shader> LoadShader(const std::string& singleFileShaderPath, const std::string& name);
            static std::shared_ptr<Rendering::Shader> GetOrLoadShader(const std::string& singleFileShaderPath, const std::string& name);
            static std::shared_ptr<Rendering::Shader> GetShader(const std::string& name);
            static std::shared_ptr<Rendering::Material> CreateMaterial(const std::string& name);
            static std::shared_ptr<Rendering::Material> CreateMaterial(const std::string& name, const std::string& shaderName);
            static std::shared_ptr<Rendering::Material> GetOrCreateMaterial(const std::string& name, const std::string& shaderName);
            static std::shared_ptr<Rendering::Material> GetMaterial(const std::string& name);
            static const std::unordered_map<std::string, std::shared_ptr<Rendering::Material>>& GetAllMaterials();
            static unsigned int GetNextMaterialId();

            static std::shared_ptr<Rendering::Texture> LoadTexture(const std::string& filePath, const std::string& name, const Rendering::TextureSettings& settings, bool bFlipVertically = true);
            static std::shared_ptr<Rendering::Texture> GetOrLoadTexture(const std::string& filePath, const std::string& name, const Rendering::TextureSettings& settings, bool bFlipVertically = true);
            static std::shared_ptr<Rendering::Texture> GetTexture(const std::string& name);
            static std::shared_ptr<Rendering::Cubemap> LoadCubemap(const Rendering::CubemapLoadSettings& loadSettings, const std::string& name);
            static std::shared_ptr<Rendering::Cubemap> GetOrLoadCubemap(const Rendering::CubemapLoadSettings& loadSettings, const std::string& name);
            static std::shared_ptr<Rendering::Cubemap> GetCubemap(const std::string& name);

            static std::shared_ptr<Rendering::Mesh> GetMesh(const std::string& name);
            static std::shared_ptr<Rendering::ModelData> LoadModel(const std::string& filePath, const std::string& name);
            static std::shared_ptr<Rendering::ModelData> GetOrLoadModel(const std::string& filePath, const std::string& name);
            static std::shared_ptr<Rendering::ModelData> GetModel(const std::string& name);
    
            static void UnloadAll();

        private:

            static std::string DEFAULT_SHADER_NAME;
            static std::string DEFAULT_MATERIAL_NAME;
            static std::string DEFAULT_MESH_CUBE_NAME;
            static std::string DEFAULT_MESH_QUAD_NAME;
            static std::string DEFAULT_MESH_SPHERE_NAME;

            static std::unordered_map<std::string, std::shared_ptr<Rendering::Shader>> m_Shaders;
            static std::unordered_map<std::string, std::shared_ptr<Rendering::Mesh>> m_Meshes;
            static std::unordered_map<std::string, std::shared_ptr<Rendering::Material>> m_Materials;
            static std::unordered_map<std::string, std::shared_ptr<Rendering::Texture>> m_Textures;
            static std::unordered_map<std::string, std::shared_ptr<Rendering::ModelData>> m_Models;
            static std::unordered_map<std::string, std::shared_ptr<Rendering::Cubemap>> m_Cubemaps;
    
            static unsigned int m_NextMaterialID;

            ResourceManager() = default;
        };
    }
}
