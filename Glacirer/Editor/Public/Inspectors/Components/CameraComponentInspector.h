#pragma once
#include "IComponentInspector.h"

namespace Glacirer
{
    class CameraComponent;
}

namespace GlacirerEditor
{
    namespace Inspector
    {
        class CameraComponentInspector : public IComponentInspector
        {
        public:

            void RenderGUI(const std::shared_ptr<Glacirer::Component>& component) override;

            static int GetComponentHash();

        private:

            void RenderOrthographicGUI(Glacirer::CameraComponent& camera);
            void RenderPerspectiveGUI(Glacirer::CameraComponent& camera);
        };
    }
}
