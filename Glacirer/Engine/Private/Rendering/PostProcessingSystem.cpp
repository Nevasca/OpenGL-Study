#include "Rendering/PostProcessingSystem.h"

#include "Rendering/FrameBuffer.h"
#include "Rendering/Material.h"
#include "Rendering/Primitive.h"
#include "ResourceManager.h"
#include "Basics/Components/PostProcessingComponent.h"

PostProcessingSystem::PostProcessingSystem()
{
    m_ScreenQuad = Primitive::CreateScreenQuad();

    const std::string POST_PROCESSING_SHADER_NAME = "PostProcessing";
    ResourceManager::LoadShader("res/core/shaders/PostProcessing.glsl", POST_PROCESSING_SHADER_NAME);

    m_PostProcessingMaterial = ResourceManager::CreateMaterial("M_PostProcessing", POST_PROCESSING_SHADER_NAME);
}

void PostProcessingSystem::SetFramebuffer(const Framebuffer& framebuffer) const
{
    m_PostProcessingMaterial->SetTexture("u_ScreenTexture", framebuffer.GetMainColorBufferTexture(), 0);
}

void PostProcessingSystem::SetPostProcessingComponent(const std::shared_ptr<PostProcessingComponent>& postProcessingComponent)
{
    m_PostProcessingComponent = postProcessingComponent;

    UpdatePostProcessingMaterial();
    m_PostProcessingComponent->SetDirty(false); 
}

void PostProcessingSystem::RemovePostProcessingComponent(const std::shared_ptr<PostProcessingComponent>& postProcessingComponent)
{
    if(m_PostProcessingComponent != postProcessingComponent)
    {
        return;
    }

    m_PostProcessingComponent = nullptr;
    UpdatePostProcessingMaterial();
}

void PostProcessingSystem::RenderToScreen()
{
    if(m_PostProcessingComponent && m_PostProcessingComponent->IsDirty())
    {
        UpdatePostProcessingMaterial();
        m_PostProcessingComponent->SetDirty(false);
    }
    
    m_MeshRenderer.Render(*m_ScreenQuad, *m_PostProcessingMaterial);
}

void PostProcessingSystem::UpdatePostProcessingMaterial()
{
    float gammaValue = 2.2f;
    bool bIsColorInversionEnabled = false;
    bool bIsGrayScaleEnabled = false;
    bool bIsSharpenEnabled = false;
    bool bIsBlurEnabled = false;
    bool bIsEdgeDetectionEnabled = false;

    if(m_PostProcessingComponent)
    {
        gammaValue = m_PostProcessingComponent->GetGammaValue();
        bIsColorInversionEnabled = m_PostProcessingComponent->IsColorInversionEnabled();
        bIsGrayScaleEnabled = m_PostProcessingComponent->IsGrayScaleEnabled();
        bIsSharpenEnabled = m_PostProcessingComponent->IsSharpenEnabled();
        bIsBlurEnabled = m_PostProcessingComponent->IsBlurEnabled();
        bIsEdgeDetectionEnabled = m_PostProcessingComponent->IsEdgeDetectionEnabled();
    }

    m_PostProcessingMaterial->SetFloat("u_PostProcessing.Gamma", gammaValue);
    m_PostProcessingMaterial->SetBool("u_PostProcessing.ColorInversionEnabled", bIsColorInversionEnabled);
    m_PostProcessingMaterial->SetBool("u_PostProcessing.GrayScaleEnabled", bIsGrayScaleEnabled);
    m_PostProcessingMaterial->SetBool("u_PostProcessing.SharpenEnabled", bIsSharpenEnabled);
    m_PostProcessingMaterial->SetBool("u_PostProcessing.BlurEnabled", bIsBlurEnabled);
    m_PostProcessingMaterial->SetBool("u_PostProcessing.EdgeDetectionEnabled", bIsEdgeDetectionEnabled);
}
