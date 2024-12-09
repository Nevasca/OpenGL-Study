#pragma once
#include <memory>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace Glacirer
{
    namespace Rendering
    {
        class ModelData;
        class Mesh;
    }

    namespace Resources
    {
        class MeshResource
        {
        public:

            static std::shared_ptr<Rendering::Mesh> LoadCube();
            static std::shared_ptr<Rendering::Mesh> LoadQuad();
            static std::shared_ptr<Rendering::Mesh> LoadSphere(const std::string& filePath);
            static std::shared_ptr<Rendering::ModelData> LoadModelFromFile(const std::string& filePath);
                                              
        private:

            static void ProcessNode(aiNode* node, const aiScene* scene, Rendering::ModelData& outModel);
            static std::shared_ptr<Rendering::Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
        };
    }
}
