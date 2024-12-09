#include "Inspectors/Components/MeshComponentInspector.h"

#include "Basics/Components/MeshComponent.h"
#include "Rendering/Material.h"
#include "Rendering/Mesh.h"
#include "imgui/imgui.h"

namespace GlacirerEditor
{
    namespace Inspector
    {
        void MeshComponentInspector::RenderGUI(const std::shared_ptr<Glacirer::Component>& component)
        {
            std::shared_ptr<Glacirer::MeshComponent> meshComponent = std::dynamic_pointer_cast<Glacirer::MeshComponent>(component);

            assert(meshComponent);

            std::shared_ptr<Glacirer::Rendering::Mesh> mesh = meshComponent->GetMesh();
            const std::string meshName = mesh ? mesh->GetName() : "None";

            ImGui::Text("Mesh: %s", meshName.c_str());

            std::shared_ptr<Glacirer::Rendering::Material> material = meshComponent->GetMaterial();
            const std::string materialName = material ? material->GetName() : "None";

            ImGui::Text("Material: %s", materialName.c_str());

            bool bIsOutlined = meshComponent->IsOutlined();
            ImGui::Checkbox("Outlined", &bIsOutlined);

            meshComponent->SetIsOutlined(bIsOutlined);
        }

        int MeshComponentInspector::GetComponentHash()
        {
            return Glacirer::MeshComponent::GetClassHash();
        }
    }
}
