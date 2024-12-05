#pragma once
#include <memory>
#include <unordered_map>
#include "IComponentInspector.h"

class GameObject;

namespace Editor
{
    class GameObjectInspector
    {
    public:
        GameObjectInspector();

        void RenderGUI(GameObject& gameObject);

    private:

        void RegisterComponentInspectors();
        void RenderTransformGUI(GameObject& gameObject);
        void RenderComponentsGUI(GameObject& gameObject);

        std::unordered_map<int, std::unique_ptr<IComponentInspector>> m_ComponentInspectorMapping{};
    };
}
