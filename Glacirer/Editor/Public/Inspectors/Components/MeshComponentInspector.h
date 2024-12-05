#pragma once
#include "IComponentInspector.h"

namespace Editor
{
    namespace Inspector
    {
        class MeshComponentInspector : public IComponentInspector
        {
        public:
            void RenderGUI(const std::shared_ptr<Component>& component) override;

            static int GetComponentHash();
        };
    }
}
