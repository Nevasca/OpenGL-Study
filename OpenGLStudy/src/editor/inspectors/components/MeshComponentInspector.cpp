#include "MeshComponentInspector.h"

#include "core/Basics/Components/MeshComponent.h"
#include "core/Rendering/Material.h"
#include "core/Rendering/Mesh.h"
#include "imgui/imgui.h"

namespace Editor
{
    namespace Inspector
    {
        void MeshComponentInspector::RenderGUI(const std::shared_ptr<Component>& component)
        {
            std::shared_ptr<MeshComponent> meshComponent = std::dynamic_pointer_cast<MeshComponent>(component);

            assert(meshComponent);

            std::shared_ptr<Mesh> mesh = meshComponent->GetMesh();
            const std::string meshName = mesh ? mesh->GetName() : "None";

            ImGui::Text("Mesh: %s", meshName.c_str());

            std::shared_ptr<Material> material = meshComponent->GetMaterial();
            const std::string materialName = material ? material->GetName() : "None";

            ImGui::Text("Material: %s", materialName.c_str());

            bool bIsOutlined = meshComponent->IsOutlined();
            ImGui::Checkbox("Outlined", &bIsOutlined);

            meshComponent->SetIsOutlined(bIsOutlined);
        }

        int MeshComponentInspector::GetComponentHash()
        {
            return MeshComponent::GetClassHash();
        }
    }
}
