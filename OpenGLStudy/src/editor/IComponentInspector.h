#pragma once
#include <memory>

class Component;

namespace Editor
{
    class IComponentInspector
    {
    public:

        virtual void RenderGUI(const std::shared_ptr<Component>& component) = 0;

        virtual ~IComponentInspector() = default;

    protected:

        IComponentInspector() = default;
    };
}
