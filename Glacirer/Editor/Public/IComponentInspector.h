#pragma once
#include <memory>

namespace Glacirer
{
    class Component;
}

namespace GlacirerEditor
{
    class IComponentInspector
    {
    public:

        virtual void RenderGUI(const std::shared_ptr<Glacirer::Component>& component) = 0;

        virtual ~IComponentInspector() = default;

    protected:

        IComponentInspector() = default;
    };
}
