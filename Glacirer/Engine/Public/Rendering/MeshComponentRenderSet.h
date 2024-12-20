#pragma once
#include <map>
#include <memory>
#include <vector>

#include <glm/vec3.hpp>

namespace Glacirer
{
    class MeshComponent;

    namespace Rendering
    {
        class Material;
        class InstancedArray;

        struct MeshComponentRenderElement
        {
            unsigned int VaoId;
            unsigned int MaterialId;
            std::shared_ptr<MeshComponent> MeshComponent;
        };
    
        class MeshComponentRenderSet
        {
        public:

            void Add(const std::shared_ptr<MeshComponent>& meshComponent, InstancedArray& instancedArray);
            void Remove(const std::shared_ptr<MeshComponent>& meshComponent);
            std::vector<std::shared_ptr<MeshComponent>> GetAllMeshComponentsUsing(const std::shared_ptr<Material>& material);
            void OverrideAllObjectsScale(const glm::vec3& scaleToAdd);
            void MultiplyAllObjectsScaleBy(const float factor);
            void DivideAllObjectsScaleBy(const float factor);
            std::multimap<float, MeshComponentRenderElement> GetMeshComponentsSortedByDistance(const glm::vec3& cameraPosition) const;
            void Clear();

            const std::map<unsigned int, std::map<unsigned int, std::vector<std::shared_ptr<MeshComponent>>>>& GetMeshComponents() const { return m_MeshComponents; }
            bool IsEmpty() const { return m_TotalMeshComponents == 0; }

        private:

            std::map<unsigned int, std::map<unsigned int, std::vector<std::shared_ptr<MeshComponent>>>> m_MeshComponents{}; // keyed by VAO and material ID
            unsigned int m_TotalMeshComponents{0};
        };
    }
}
