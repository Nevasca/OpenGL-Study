#include "PostProcessingComponent.h"

#include "core/World.h"
#include "core/GameObject/GameObject.h"

void PostProcessingComponent::Enable()
{
    Component::Enable();

    World& world = GetOwner().GetWorld();
    world.SetPostProcessingComponent(GetThis());
}

void PostProcessingComponent::Disable()
{
    Component::Disable();

    World& world = GetOwner().GetWorld();
    world.RemovePostProcessingComponent(GetThis());
}

void PostProcessingComponent::SetGammaValue(const float gammaValue)
{
    m_GammaValue = gammaValue;
    bIsDirty = true;
}

void PostProcessingComponent::SetColorInversion(const bool bEnabled)
{
    bIsColorInversionEnabled = bEnabled;
    bIsDirty = true;
}

void PostProcessingComponent::SetGrayScale(const bool bEnabled)
{
    bIsGrayScaleEnabled = bEnabled;
    bIsDirty = true;
}

void PostProcessingComponent::SetSharpen(const bool bEnabled)
{
    bIsSharpenEnabled = bEnabled;
    bIsDirty = true;
}

void PostProcessingComponent::SetBlur(const bool bEnabled)
{
    bIsBlurEnabled = bEnabled;
    bIsDirty = true;
}

void PostProcessingComponent::SetEdgeDetection(const bool bEnabled)
{
    bIsEdgeDetectionEnabled = bEnabled;
    bIsDirty = true;
}
