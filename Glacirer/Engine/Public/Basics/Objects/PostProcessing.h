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

        void SetColorInversion(const bool bEnabled) const;
        void SetGrayScale(const bool bEnabled) const;
        void SetSharpen(const bool bEnabled) const;
        void SetBlur(const bool bEnabled) const;
        void SetEdgeDetection(const bool bEnabled) const;

    private:

        std::weak_ptr<PostProcessingComponent> m_PostProcessingComponent{};
    };
}
