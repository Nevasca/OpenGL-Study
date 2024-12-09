#pragma once
#include "GameObject/GameObject.h"
#include "Basics/Components/PostProcessingComponent.h"

namespace Glacirer
{
    class ENGINE_API PostProcessing : public GameObject
    {
        GENERATE_OBJECT_BODY(PostProcessing)

    public:

        void Initialize() override;

        void SetColorInversion(const bool bEnabled) const { m_PostProcessingComponent->SetColorInversion(bEnabled); }
        void SetGrayScale(const bool bEnabled) const { m_PostProcessingComponent->SetGrayScale(bEnabled); }
        void SetSharpen(const bool bEnabled) const { m_PostProcessingComponent->SetSharpen(bEnabled); }
        void SetBlur(const bool bEnabled) const { m_PostProcessingComponent->SetBlur(bEnabled); }
        void SetEdgeDetection(const bool bEnabled) const { m_PostProcessingComponent->SetEdgeDetection(bEnabled); }

    private:

        std::shared_ptr<PostProcessingComponent> m_PostProcessingComponent{};
    };
}
