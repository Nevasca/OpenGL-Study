#include "Basics/Objects/PostProcessing.h"

namespace Glacirer
{
    void PostProcessing::Initialize()
    {
        GameObject::Initialize();

        m_PostProcessingComponent = AddComponent<PostProcessingComponent>();
    }

    void PostProcessing::SetColorInversion(const bool bEnabled) const
    {
        std::shared_ptr<PostProcessingComponent> postProcessingComponent = m_PostProcessingComponent.lock();

        if(postProcessingComponent)
        {
            postProcessingComponent->SetColorInversion(bEnabled);
        }
    }

    void PostProcessing::SetGrayScale(const bool bEnabled) const
    {
        std::shared_ptr<PostProcessingComponent> postProcessingComponent = m_PostProcessingComponent.lock();

        if(postProcessingComponent)
        {
            postProcessingComponent->SetGrayScale(bEnabled);
        }
    }

    void PostProcessing::SetSharpen(const bool bEnabled) const
    {
        std::shared_ptr<PostProcessingComponent> postProcessingComponent = m_PostProcessingComponent.lock();

        if(postProcessingComponent)
        {
            postProcessingComponent->SetSharpen(bEnabled);
        }
    }

    void PostProcessing::SetBlur(const bool bEnabled) const
    {
        std::shared_ptr<PostProcessingComponent> postProcessingComponent = m_PostProcessingComponent.lock();

        if(postProcessingComponent)
        {
            postProcessingComponent->SetBlur(bEnabled);
        }
    }

    void PostProcessing::SetEdgeDetection(const bool bEnabled) const
    {
        std::shared_ptr<PostProcessingComponent> postProcessingComponent = m_PostProcessingComponent.lock();

        if(postProcessingComponent)
        {
            postProcessingComponent->SetEdgeDetection(bEnabled);
        }
    }
}
