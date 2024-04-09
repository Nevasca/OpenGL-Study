#include "GameObjectInspector.h"

#include "core/GameObject/GameObject.h"
#include "core/GameObject/Transform.h"
#include <imgui/imgui.h>

namespace Editor
{
    void GameObjectInspector::RenderGUI(GameObject& gameObject)
    {
        ImGui::Text("%s", gameObject.GetName().c_str());

        Transform& transform = gameObject.GetTransform();

        glm::vec3 position = transform.GetPosition();
        glm::vec3 rotation = transform.GetRotation();
        glm::vec3 scale = transform.GetScale();

        ImGui::DragFloat3("Position", &position.x);
        ImGui::DragFloat3("Rotation", &rotation.x);
        ImGui::DragFloat3("Scale", &scale.x);

        transform.SetPositionRotationScale(position, rotation, scale);
    }
}
