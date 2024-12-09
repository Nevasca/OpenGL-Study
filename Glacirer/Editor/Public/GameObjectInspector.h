#pragma once
#include <memory>
#include <unordered_map>
#include "IComponentInspector.h"

namespace Glacirer
{
    class GameObject;
}

namespace GlacirerEditor
{
    class GameObjectInspector
    {
    public:
        GameObjectInspector();

        void RenderGUI(Glacirer::GameObject& gameObject);

    private:

        void RegisterComponentInspectors();
        void RenderTransformGUI(Glacirer::GameObject& gameObject);
        void RenderComponentsGUI(Glacirer::GameObject& gameObject);

        std::unordered_map<int, std::unique_ptr<IComponentInspector>> m_ComponentInspectorMapping{};
    };
}
