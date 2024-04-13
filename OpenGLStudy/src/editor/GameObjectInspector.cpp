#include "GameObjectInspector.h"

#include "core/GameObject/GameObject.h"
#include "core/GameObject/Transform.h"
#include "core/GameObject/Component.h"
#include <imgui/imgui.h>

#include "inspectors/CameraComponentInspector.h"
#include "inspectors/DirectionalLightComponentInspector.h"
#include "inspectors/MeshComponentInspector.h"
#include "inspectors/PilotComponentInspector.h"
#include "inspectors/PointLightComponentInspector.h"
#include "inspectors/SpotLightComponentInspector.h"

namespace Editor
{
    GameObjectInspector::GameObjectInspector()
    {
        RegisterComponentInspectors();
    }

    void GameObjectInspector::RenderGUI(GameObject& gameObject)
    {
        ImGui::SeparatorText(gameObject.GetName().c_str());

        RenderTransformGUI(gameObject);
        RenderComponentsGUI(gameObject);
    }

    void GameObjectInspector::RenderTransformGUI(GameObject& gameObject)
    {
        Transform& transform = gameObject.GetTransform();

        glm::vec3 position = transform.GetPosition();
        glm::vec3 rotation = transform.GetRotation();
        glm::vec3 scale = transform.GetScale();

        ImGui::DragFloat3("Position", &position.x);
        ImGui::DragFloat3("Rotation", &rotation.x);
        ImGui::DragFloat3("Scale", &scale.x);

        transform.SetPositionRotationScale(position, rotation, scale);
    }

    void GameObjectInspector::RenderComponentsGUI(GameObject& gameObject)
    {
        ImGui::SeparatorText("Components");

        std::vector<std::shared_ptr<Component>> components = gameObject.GetComponents();

        for(int i = 0; i < static_cast<int>(components.size()); i++)
        {
            std::string componentLabel = components[i]->GetName() + std::to_string(i);

            if(!ImGui::CollapsingHeader(componentLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
            {
                continue;
            }

            int componentHash = components[i]->GetHash();

            if(m_ComponentInspectorMapping.find(componentHash) != m_ComponentInspectorMapping.end())
            {
                m_ComponentInspectorMapping[componentHash]->RenderGUI(components[i]);
            }
        }
    }

    void GameObjectInspector::RegisterComponentInspectors()
    {
        using namespace Inspector;

        m_ComponentInspectorMapping[DirectionalLightComponentInspector::GetComponentHash()] = std::make_unique<DirectionalLightComponentInspector>();
        m_ComponentInspectorMapping[PointLightComponentInspector::GetComponentHash()] = std::make_unique<PointLightComponentInspector>();
        m_ComponentInspectorMapping[SpotLightComponentInspector::GetComponentHash()] = std::make_unique<SpotLightComponentInspector>();
        m_ComponentInspectorMapping[CameraComponentInspector::GetComponentHash()] = std::make_unique<CameraComponentInspector>();
        m_ComponentInspectorMapping[PilotComponentInspector::GetComponentHash()] = std::make_unique<PilotComponentInspector>();
        m_ComponentInspectorMapping[MeshComponentInspector::GetComponentHash()] = std::make_unique<MeshComponentInspector>();
    }
}
