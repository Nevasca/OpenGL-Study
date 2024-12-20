#include "Resources/MeshResource.h"

#include <iostream>
#include <memory>
#include <assimp/postprocess.h>

#include "Rendering/Mesh.h"
#include "Rendering/ModelData.h"
#include "Rendering/Primitive.h"

namespace Glacirer
{
    namespace Resources
    {
        std::shared_ptr<Rendering::Mesh> MeshResource::LoadCube()
        {
            return Rendering::Primitive::CreateCube();
        }

        std::shared_ptr<Rendering::Mesh> MeshResource::LoadQuad()
        {
            return Rendering::Primitive::CreateQuad();
        }

        std::shared_ptr<Rendering::Mesh> MeshResource::LoadSphere(const std::string& filePath)
        {
            // Load sphere from primitive model file and return it as a single mesh
            const std::shared_ptr<Rendering::ModelData> sphereModel = LoadModelFromFile(filePath);

            const std::vector<std::shared_ptr<Rendering::Mesh>>& sphereMeshes = sphereModel->GetMeshes();
            assert(sphereMeshes.size() == 1);

            return sphereMeshes[0];
        }

        std::shared_ptr<Rendering::ModelData> MeshResource::LoadModelFromFile(const std::string& filePath)
        {
            Assimp::Importer importer;

            // path
            // pFlags: desired post-processing options
            //  aiProcess_Triangulate: if the model doesn't (entirely) consist of triangles, transform model primitives to triangles first
            //  aiProcess_FlipUVs: image is flipped on OpenGL, so we need to flip UV (no longer using it, flipping all textures to keep consistency when loading textures)
            // Mode post=processing options: https://assimp.sourceforge.net/lib_html/postprocess_8h.html
            const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate);

            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                std::cout << "ERROR::ASSIMP: " << importer.GetErrorString() << "\n";
                return std::make_shared<Rendering::ModelData>();
            }

            std::shared_ptr<Rendering::ModelData> model = std::make_shared<Rendering::ModelData>();

#ifdef _DEBUG
            std::cout << "Processing scene " << scene->mName.C_Str() << "\n";
#endif
    
            ProcessNode(scene->mRootNode, scene, *model);

            return model;
        }

        // Recursively process node and children nodes
        void MeshResource::ProcessNode(aiNode* node, const aiScene* scene, Rendering::ModelData& outModel)
        {
#ifdef _DEBUG
            std::cout << "Processing node " << node->mName.C_Str() << "\n";
#endif

            // Process all the node's meshes if any
            for(unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                // Nodes contains just an index of the mesh, scene is the one that holds the meshes data
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        
                std::shared_ptr<Rendering::Mesh> processedMesh = ProcessMesh(mesh, scene);
                processedMesh->SetName(node->mName.C_Str());
                outModel.AddMesh(std::move(processedMesh));
            }

            // Then do the same for each of its children
            for(unsigned int i = 0; i < node->mNumChildren; i++)
            {
                ProcessNode(node->mChildren[i], scene, outModel);
            }

            // If we just iterated on scene->mMeshes we would lose the possibility of setting a parent children relationship
            // on meshes, where children move together with parent, for instance
            // this example doesn't use this, but it's nice to keep this approach for that
        }

        std::shared_ptr<Rendering::Mesh> MeshResource::ProcessMesh(aiMesh* mesh, const aiScene* scene)
        {
#ifdef _DEBUG           
            std::cout << "Processing mesh " << mesh->mName.C_Str() << "\n";
#endif

            std::vector<Rendering::Vertex> vertices{};
            vertices.reserve(mesh->mNumVertices);

            for(unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                // Process vertex positions, normals and texture coordinates
                Rendering::Vertex vertex{};
        
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
                    glm::vec2 texCoord{};
                    texCoord.x = mesh->mTextureCoords[0][i].x;
                    texCoord.y = mesh->mTextureCoords[0][i].y;
                    vertex.TexCoord = texCoord;
                }
                else
                {
                    vertex.TexCoord = glm::vec2{0.f, 0.f};
                }

                vertices.emplace_back(vertex);
            }

            std::vector<unsigned int> indices{};
            indices.reserve(mesh->mNumFaces);
    
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

            return std::make_shared<Rendering::Mesh>(vertices, indices);
        }
    }
}
