#include "Basics/Objects/Model.h"

#include "Basics/Components/MeshComponent.h"
#include "Rendering/ModelData.h"

namespace Glacirer
{
    void Model::Setup(const std::shared_ptr<Rendering::ModelData>& modelData, const std::shared_ptr<Rendering::Material>& material)
    {
        for (const std::shared_ptr<Rendering::Mesh>& mesh : modelData->GetMeshes())
        {
            std::weak_ptr<MeshComponent> component = AddComponent<MeshComponent>();
            std::shared_ptr<MeshComponent> meshComponent = component.lock();
            assert(meshComponent);

            meshComponent->SetMesh(mesh);
            meshComponent->SetMaterial(material);
        }
    }
}
