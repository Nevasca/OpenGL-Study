#include "Inspectors/Components/MeshComponentInspector.h"

#include "IMaterialProvider.h"
#include "Basics/Components/MeshComponent.h"
#include "Rendering/Material.h"
#include "Rendering/Mesh.h"
#include "imgui/imgui.h"
#include "Resources/ResourceManager.h"

namespace GlacirerEditor
{
    namespace Inspector
    {
        MeshComponentInspector::MeshComponentInspector(const std::weak_ptr<IMaterialProvider>& materialProvider)
            : m_MaterialProvider(materialProvider)
        { }

        void MeshComponentInspector::RenderGUI(const std::shared_ptr<Glacirer::Component>& component)
        {
            std::shared_ptr<Glacirer::MeshComponent> meshComponent = std::dynamic_pointer_cast<Glacirer::MeshComponent>(component);

            assert(meshComponent);

            std::shared_ptr<Glacirer::Rendering::Mesh> mesh = meshComponent->GetMesh();
            const std::string meshName = mesh ? mesh->GetName() : "None";

            ImGui::Text("Mesh: %s", meshName.c_str());

            RenderMaterialGUI(*meshComponent);

            bool bIsOutlined = meshComponent->IsOutlined();
            ImGui::Checkbox("Outlined", &bIsOutlined);

            meshComponent->SetIsOutlined(bIsOutlined);
        }

        int MeshComponentInspector::GetComponentHash()
        {
            return Glacirer::MeshComponent::GetClassHash();
        }

        void MeshComponentInspector::RenderMaterialGUI(Glacirer::MeshComponent& meshComponent)
        {
            std::shared_ptr<Glacirer::Rendering::Material> material = meshComponent.GetMaterial();
            const std::string materialName = material ? material->GetName() : "None";

            ImGui::Text("Material: %s", materialName.c_str());
            ImGui::SameLine();

            std::string buttonLabel = "Apply selected##" + std::to_string(meshComponent.GetId());
            if(!ImGui::Button(buttonLabel.c_str()))
            {
                return;
            }

            std::shared_ptr<IMaterialProvider> materialProvider = m_MaterialProvider.lock();

            if(!materialProvider || !materialProvider->HasAnyMaterialSelected())
            {
                return;
            }

            std::string selectedMaterialName = materialProvider->GetCurrentSelectedMaterialName();
            std::shared_ptr<Glacirer::Rendering::Material> selectedMaterial = Glacirer::Resources::ResourceManager::GetMaterial(selectedMaterialName);
            meshComponent.SetMaterial(selectedMaterial);
        }
    }
}
