#include "Inspectors/Components/PostProcessingComponentInspector.h"

#include "Basics/Components/PostProcessingComponent.h"
#include "imgui/imgui.h"

namespace Editor
{
    namespace Inspector
    {
        void PostProcessingComponentInspector::RenderGUI(const std::shared_ptr<Component>& component)
        {
            std::shared_ptr<PostProcessingComponent> postProcessing = std::dynamic_pointer_cast<PostProcessingComponent>(component);
            assert(postProcessing);

            float gammaValue = postProcessing->GetGammaValue();
            ImGui::DragFloat("Gamma Value", &gammaValue, 0.1f);
            
            bool bIsColorInversionEnabled = postProcessing->IsColorInversionEnabled();
            ImGui::Checkbox("Color Inversion", &bIsColorInversionEnabled);

            bool bIsGrayScaleEnabled = postProcessing->IsGrayScaleEnabled();
            ImGui::Checkbox("Grayscale", &bIsGrayScaleEnabled);

            bool bIsSharpenEnabled = postProcessing->IsSharpenEnabled();
            ImGui::Checkbox("Sharpen", &bIsSharpenEnabled);

            bool bIsBlurEnabled = postProcessing->IsBlurEnabled();
            ImGui::Checkbox("Blur", &bIsBlurEnabled);

            bool bIsEdgeDetectionEnabled = postProcessing->IsEdgeDetectionEnabled();
            ImGui::Checkbox("Edge Detection", &bIsEdgeDetectionEnabled);

            postProcessing->SetGammaValue(gammaValue);
            postProcessing->SetColorInversion(bIsColorInversionEnabled);
            postProcessing->SetGrayScale(bIsGrayScaleEnabled);
            postProcessing->SetSharpen(bIsSharpenEnabled);
            postProcessing->SetBlur(bIsBlurEnabled);
            postProcessing->SetEdgeDetection(bIsEdgeDetectionEnabled);
        }

        int PostProcessingComponentInspector::GetComponentHash()
        {
            return PostProcessingComponent::GetClassHash();
        }
    }
}
