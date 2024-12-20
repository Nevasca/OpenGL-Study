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
    class IMaterialProvider;

    class GameObjectInspector
    {
    public:
        void Setup(const std::weak_ptr<IMaterialProvider>& materialProvider);
        void RenderGUI(Glacirer::GameObject& gameObject);

    private:
        void RegisterComponentInspectors(const std::weak_ptr<IMaterialProvider>& materialProvider);
        void RenderTransformGUI(Glacirer::GameObject& gameObject);
        void RenderComponentsGUI(Glacirer::GameObject& gameObject);

        std::unordered_map<int, std::unique_ptr<IComponentInspector>> m_ComponentInspectorMapping{};
    };
}
