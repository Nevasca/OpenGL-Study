#pragma once
#include "editor/IComponentInspector.h"

namespace Editor
{
    namespace Inspector
    {
        class CameraComponentInspector : public IComponentInspector
        {
        public:

            void RenderGUI(const std::shared_ptr<Component>& component) override;

            static int GetComponentHash();
        };
    }
}
