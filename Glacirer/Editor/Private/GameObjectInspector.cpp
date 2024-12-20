#include "GameObjectInspector.h"

#include "GameObject/GameObject.h"
#include "GameObject/Transform.h"
#include "GameObject/Component.h"
#include <imgui/imgui.h>

#include "inspectors/components/CameraComponentInspector.h"
#include "inspectors/components/DirectionalLightComponentInspector.h"
#include "inspectors/components/MeshComponentInspector.h"
#include "inspectors/components/PilotCameraControllerInspector.h"
#include "inspectors/components/PilotComponentInspector.h"
#include "inspectors/components/PointLightComponentInspector.h"
#include "inspectors/components/PostProcessingComponentInspector.h"
#include "inspectors/components/SpotLightComponentInspector.h"

namespace GlacirerEditor
{
    void GameObjectInspector::Setup(const std::weak_ptr<IMaterialProvider>& materialProvider)
    {
        RegisterComponentInspectors(materialProvider);
    }

    void GameObjectInspector::RenderGUI(Glacirer::GameObject& gameObject)
    {
        ImGui::SeparatorText(gameObject.GetName().c_str());

        RenderTransformGUI(gameObject);
        RenderComponentsGUI(gameObject);
    }

    void GameObjectInspector::RenderTransformGUI(Glacirer::GameObject& gameObject)
    {
        Glacirer::Transform& transform = gameObject.GetTransform();

        glm::vec3 position = transform.GetPosition();
        glm::vec3 rotation = transform.GetRotation();
        glm::vec3 scale = transform.GetScale();

        ImGui::DragFloat3("Position", &position.x);
        ImGui::DragFloat3("Rotation", &rotation.x);
        ImGui::DragFloat3("Scale", &scale.x);

        transform.SetPositionRotationScale(position, rotation, scale);
    }

    void GameObjectInspector::RenderComponentsGUI(Glacirer::GameObject& gameObject)
    {
        ImGui::SeparatorText("Components");

        std::vector<std::shared_ptr<Glacirer::Component>> components = gameObject.GetComponents();

        for(int i = 0; i < static_cast<int>(components.size()); i++)
        {
            std::string componentLabel = components[i]->GetName() + "##" + std::to_string(i);

            bool bAdded = true;
            
            if(ImGui::CollapsingHeader(componentLabel.c_str(), &bAdded, ImGuiTreeNodeFlags_DefaultOpen))
            {
                int componentHash = components[i]->GetHash();

                if(m_ComponentInspectorMapping.find(componentHash) != m_ComponentInspectorMapping.end())
                {
                    m_ComponentInspectorMapping[componentHash]->RenderGUI(components[i]);
                }
            }

            if (!bAdded)
            {
                gameObject.RemoveComponent(components[i]);
            }
        }
    }

    void GameObjectInspector::RegisterComponentInspectors(const std::weak_ptr<IMaterialProvider>& materialProvider)
    {
        using namespace Inspector;

        m_ComponentInspectorMapping[DirectionalLightComponentInspector::GetComponentHash()] = std::make_unique<DirectionalLightComponentInspector>();
        m_ComponentInspectorMapping[PointLightComponentInspector::GetComponentHash()] = std::make_unique<PointLightComponentInspector>();
        m_ComponentInspectorMapping[SpotLightComponentInspector::GetComponentHash()] = std::make_unique<SpotLightComponentInspector>();
        m_ComponentInspectorMapping[CameraComponentInspector::GetComponentHash()] = std::make_unique<CameraComponentInspector>();
        m_ComponentInspectorMapping[PilotComponentInspector::GetComponentHash()] = std::make_unique<PilotComponentInspector>();
        m_ComponentInspectorMapping[MeshComponentInspector::GetComponentHash()] = std::make_unique<MeshComponentInspector>(materialProvider);
        m_ComponentInspectorMapping[PostProcessingComponentInspector::GetComponentHash()] = std::make_unique<PostProcessingComponentInspector>();
        m_ComponentInspectorMapping[PilotCameraControllerInspector::GetComponentHash()] = std::make_unique<PilotCameraControllerInspector>();
    }
}
