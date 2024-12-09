#include "Basics/Objects/Model.h"

#include "Basics/Components/MeshComponent.h"
#include "Rendering/ModelData.h"

namespace Glacirer
{
    void Model::Setup(const std::shared_ptr<Rendering::ModelData>& modelData, const std::shared_ptr<Rendering::Material>& material)
    {
        for (const std::shared_ptr<Rendering::Mesh>& mesh : modelData->GetMeshes())
        {
            std::shared_ptr<MeshComponent> meshComponent = AddComponent<MeshComponent>();
            meshComponent->SetMesh(mesh);
            meshComponent->SetMaterial(material);
        }
    }
}
