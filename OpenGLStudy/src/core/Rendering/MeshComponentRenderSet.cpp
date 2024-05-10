#include "MeshComponentRenderSet.h"

#include "InstancedArray.h"
#include "Material.h"
#include "Mesh.h"
#include "ShaderRenderSet.h"
#include "VertexArray.h"
#include "core/Basics/Components/MeshComponent.h"

class Mesh;
class Shader;
class Material;

namespace Rendering
{
    void MeshComponentRenderSet::Add(const std::shared_ptr<MeshComponent>& meshComponent, ShaderRenderSet& uniqueShadersSet, InstancedArray& instancedArray)
    {
        const std::shared_ptr<Material>& meshMaterial = meshComponent->GetMaterial();
        const unsigned int materialId = meshMaterial->GetId();
    
        const std::shared_ptr<Shader>& meshShader = meshMaterial->GetShader();
        uniqueShadersSet.Add(meshShader);

        const std::shared_ptr<Mesh>& mesh = meshComponent->GetMesh();
        VertexArray& vao = mesh->GetVertexArray();
        const unsigned int vaoID = vao.GetRendererID();

        if(m_MeshComponents.find(vaoID) == m_MeshComponents.end())
        {
            m_MeshComponents[vaoID] = {};

            instancedArray.SetupInstancedAttributesFor(mesh->GetVertexArray());
        }

        if(m_MeshComponents[vaoID].find(materialId) == m_MeshComponents[vaoID].end())
        {
            m_MeshComponents[vaoID][materialId] = {};
        }

        m_MeshComponents[vaoID][materialId].push_back(meshComponent);

        m_TotalMeshComponents++;
    }

    void MeshComponentRenderSet::Remove(const std::shared_ptr<MeshComponent>& meshComponent, ShaderRenderSet& uniqueShadersSet)
    {
        const std::shared_ptr<Material>& meshMaterial = meshComponent->GetMaterial();
        const unsigned int materialId = meshMaterial->GetId();
    
        const std::shared_ptr<Shader>& meshShader = meshMaterial->GetShader();
        uniqueShadersSet.Remove(meshShader);

        const std::shared_ptr<Mesh>& mesh = meshComponent->GetMesh();
        VertexArray& vao = mesh->GetVertexArray();
        const unsigned int vaoID = vao.GetRendererID();

        std::vector<std::shared_ptr<MeshComponent>>& components = m_MeshComponents[vaoID][materialId];
        components.erase(std::remove(components.begin(), components.end(), meshComponent), components.end());

        if(components.empty())
        {
            m_MeshComponents[vaoID].erase(materialId);

            if(m_MeshComponents[vaoID].empty())
            {
                m_MeshComponents.erase(vaoID);
            }
        }

        m_TotalMeshComponents--;
    }

    void MeshComponentRenderSet::OverrideAllObjectsScale(const glm::vec3& scaleToAdd)
    {
        for(const auto& meshMappingPair : m_MeshComponents)
        {
            for(const auto& meshComponentPair : meshMappingPair.second)
            {
                for(const auto& meshComponent : meshComponentPair.second)
                {
                    glm::vec3 originalScale = meshComponent->GetOwnerScale();
                    meshComponent->SetOwnerScale(originalScale + scaleToAdd);
                }
            }
        }
    }

    // Using multimap to avoid two objects with same distance from camera override each other on single map entry
    std::multimap<float, MeshComponentRenderElement> MeshComponentRenderSet::GetMeshComponentsSortedByDistance(const glm::vec3& cameraPosition) const
    {
        std::multimap<float, MeshComponentRenderElement> sortedElements;

        for(const auto& meshMappingPair : m_MeshComponents)
        {
            for(const auto& meshComponentPair : meshMappingPair.second)
            {
                for(const auto& meshComponent : meshComponentPair.second)
                {
                    float distanceFromCamera = glm::length(cameraPosition - meshComponent->GetOwnerPosition());

                    MeshComponentRenderElement element{meshMappingPair.first, meshComponentPair.first, meshComponent};
                    sortedElements.insert(std::pair<float, MeshComponentRenderElement>(distanceFromCamera, element));
                }
            }
        }

        return sortedElements;
    }

    void MeshComponentRenderSet::Clear()
    {
        m_MeshComponents.clear();
    }
}
