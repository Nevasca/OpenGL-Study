#pragma once

#include "IComponentInspector.h"

namespace GlacirerEditor
{
    namespace Inspector
    {
        class PointLightComponentInspector : public IComponentInspector
        {
        public:
            void RenderGUI(const std::shared_ptr<Glacirer::Component>& component) override;

            static int GetComponentHash();
        };
    }
}
