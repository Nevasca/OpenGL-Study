#pragma once
#include "core/GameObject/Component.h"

class PostProcessingComponent : public Component
{
    GENERATE_COMPONENT_BODY(PostProcessingComponent)

public:

    void Enable() override;
    void Disable() override;

    void SetColorInversion(const bool bEnabled);
    void SetGrayScale(const bool bEnabled);
    void SetSharpen(const bool bEnabled);
    void SetBlur(const bool bEnabled);
    void SetEdgeDetection(const bool bEnabled);

    bool IsColorInversionEnabled() const { return bIsColorInversionEnabled; }
    bool IsGrayScaleEnabled() const { return bIsGrayScaleEnabled; }
    bool IsSharpenEnabled() const { return bIsSharpenEnabled; }
    bool IsBlurEnabled() const { return  bIsBlurEnabled; }
    bool IsEdgeDetectionEnabled() const { return bIsEdgeDetectionEnabled; }

    void SetDirty(const bool bDirty) { bIsDirty = bDirty; }
    bool IsDirty() const { return bIsDirty; }

private:

    bool bIsColorInversionEnabled{false};
    bool bIsGrayScaleEnabled{false};
    bool bIsSharpenEnabled{false};
    bool bIsBlurEnabled{false};
    bool bIsEdgeDetectionEnabled{false};

    bool bIsDirty{true};
};
