#include "Basics/Objects/Model.h"

#include "Basics/Components/MeshComponent.h"
#include "Rendering/ModelData.h"

void Model::Setup(const std::shared_ptr<ModelData>& modelData, const std::shared_ptr<Material>& material)
{
    for (const std::shared_ptr<Mesh>& mesh : modelData->GetMeshes())
    {
        std::shared_ptr<MeshComponent> meshComponent = AddComponent<MeshComponent>();
        meshComponent->SetMesh(mesh);
        meshComponent->SetMaterial(material);
    }
}
