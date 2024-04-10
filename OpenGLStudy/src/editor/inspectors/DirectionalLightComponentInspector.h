#pragma once
#include "editor/IComponentInspector.h"
#include "core/Basics/Components/DirectionalLightComponent.h"

namespace Editor
{
    namespace Inspector
    {
        class DirectionalLightComponentInspector : public IComponentInspector
        {
        public:
            void RenderGUI(const std::shared_ptr<Component>& component) override;

            static int GetComponentHash() { return DirectionalLightComponent::GetClassHash(); }
        };
    }
}
