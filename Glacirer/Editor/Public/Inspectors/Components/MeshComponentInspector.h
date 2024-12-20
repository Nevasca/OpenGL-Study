#pragma once
#include "IComponentInspector.h"

namespace Glacirer
{
    class MeshComponent;
}

namespace GlacirerEditor
{
    class IMaterialProvider;

    namespace Inspector
    {
        class MeshComponentInspector : public IComponentInspector
        {
        public:
            MeshComponentInspector(const std::weak_ptr<IMaterialProvider>& materialProvider);

            void RenderGUI(const std::shared_ptr<Glacirer::Component>& component) override;

            static int GetComponentHash();

        private:
            void RenderMaterialGUI(Glacirer::MeshComponent& meshComponent);

            std::weak_ptr<IMaterialProvider> m_MaterialProvider;
        };
    }
}
