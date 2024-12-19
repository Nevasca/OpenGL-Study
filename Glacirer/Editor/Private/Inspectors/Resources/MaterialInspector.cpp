#include "Inspectors/Resources/MaterialInspector.h"

#include <imgui/imgui.h>

#include "Rendering/Material.h"
#include "Rendering/Shader.h"
#include "Rendering/Texture.h"
#include "Resources/ResourceManager.h"
#include "Util/FileDialog.h"

namespace GlacirerEditor
{
    namespace Inspector
    {
        constexpr ImVec2 THUMBNAIL_SIZE{100.f, 100.f};
        constexpr float THUMBNAIL_OUTLINE_THICKNESS = 4.f;
        constexpr ImVec2 EMPTY_THUMBNAIL_SIZE{THUMBNAIL_SIZE.x + THUMBNAIL_OUTLINE_THICKNESS, THUMBNAIL_SIZE.y + THUMBNAIL_OUTLINE_THICKNESS};
        
        void MaterialInspector::RenderGUI(Glacirer::Rendering::Material& material)
        {
            ImGui::SeparatorText(material.GetName().c_str());

            const std::shared_ptr<Glacirer::Rendering::Shader>& shader = material.GetShader();
            std::string shaderName = shader ? shader->GetName() : "None";
            ImGui::Text("Shader: %s", shaderName.c_str());

            RenderRenderingMode(material);
            RenderColorProperties(material);
            RenderTextureProperties(material);
            RenderBoolProperties(material);
            RenderFloatProperties(material);
            RenderIntProperties(material);
        }

        void MaterialInspector::RenderRenderingMode(Glacirer::Rendering::Material& material)
        {
            const char* renderingModes[] = { "Opaque", "AlphaCutout", "Transparent" };
            int currentModeIndex = static_cast<int>(material.GetRenderingMode()); 

            const char* currentModeName = renderingModes[currentModeIndex];

            if (ImGui::BeginCombo("Rendering Mode", currentModeName))
            {
                for (int i = 0; i < IM_ARRAYSIZE(renderingModes); i++)
                {
                    const bool isSelected = currentModeIndex == i;
                    if (ImGui::Selectable(renderingModes[i], isSelected))
                    {
                        currentModeIndex = i;
                    }

                    if (isSelected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }

            material.SetRenderingMode(static_cast<Glacirer::Rendering::MaterialRenderingMode>(currentModeIndex));
        }

        void MaterialInspector::RenderColorProperties(Glacirer::Rendering::Material& material)
        {
            const auto& materialColorProperties = material.GetAllColorProperties();
            std::map<std::string, glm::vec4> inspectorColorProperties{};

            for (const auto& colorPropertyPair : materialColorProperties)
            {
                std::string colorName = colorPropertyPair.first;
                
                inspectorColorProperties[colorName] = colorPropertyPair.second;
                ImGui::ColorEdit3(colorName.c_str(), &inspectorColorProperties[colorName].r);
            }

            for (const auto& inspectorColorPropertyPair : inspectorColorProperties)
            {
                material.SetColor(inspectorColorPropertyPair.first, inspectorColorPropertyPair.second);
            }
        }

        void MaterialInspector::RenderTextureProperties(Glacirer::Rendering::Material& material)
        {
            const auto& materialTextureProperties = material.GetAllTextureProperties();
            
            for (const auto& texturePropertyPair : materialTextureProperties)
            {
                const std::string& uniformName = texturePropertyPair.first;
                const Glacirer::Rendering::MaterialTextureProperty& textureProperty = texturePropertyPair.second;
                std::shared_ptr<Glacirer::Rendering::Texture> texture = textureProperty.Texture;

                if(texture)
                {
                    RenderAssignedTextureProperty(material, uniformName, texture, textureProperty.Slot);
                }
                else
                {
                    RenderEmptyTextureProperty(material, uniformName, textureProperty.Slot);
                }
            }
        }

        void MaterialInspector::RenderAssignedTextureProperty(
            Glacirer::Rendering::Material& material,
            const std::string& uniformName,
            const std::shared_ptr<Glacirer::Rendering::Texture>& texture,
            unsigned int slot)
        {
            ImGui::Text("%s: %s", uniformName.c_str(), texture->GetName().c_str());

            ImVec2 uv0{0, 0};
            ImVec2 uv1{1, 1};

            // If we flipped the texture on load (OpenGL generally requires that), we need to invert uv (top left v0 and top right v1)
            // to proper display it on inspectors
            if(texture->IsFlippedOnLoad())
            {
                uv0.y = 1;
                uv1.y = 0;
            }

            if(ImGui::ImageButton(uniformName.c_str(), (ImTextureID)texture->GetRendererID(), THUMBNAIL_SIZE, uv0, uv1))
            {
                TryApplyingTextureFor(material, uniformName, slot);
            }

            ImGui::SameLine();
            if(ImGui::Button("x"))
            {
                material.SetTexture(uniformName, nullptr, slot);
            }
        }

        void MaterialInspector::RenderEmptyTextureProperty(Glacirer::Rendering::Material& material, const std::string& uniformName, unsigned int slot)
        {
            ImGui::Text("%s", uniformName.c_str());

            ImGuiStyle style = ImGui::GetStyle();
            const ImU32 outlineHoveredColor = ImGui::GetColorU32(style.Colors[ImGuiCol_ButtonHovered]);
            const ImU32 outlineColor = ImGui::GetColorU32(style.Colors[ImGuiCol_FrameBg]);
            const ImU32 backgroundColor = ImGui::GetColorU32(IM_COL32(35, 35, 35, 220));

            ImDrawList* drawList = ImGui::GetWindowDrawList();
            ImVec2 positionMin = ImGui::GetCursorScreenPos();
            ImVec2 positionMax{positionMin.x + EMPTY_THUMBNAIL_SIZE.x, positionMin.y + EMPTY_THUMBNAIL_SIZE.y};
            drawList->AddRectFilled(positionMin, positionMax, backgroundColor);

            if(ImGui::IsMouseHoveringRect(positionMin, positionMax))
            {
                drawList->AddRect(positionMin, positionMax, outlineHoveredColor, 0.f, 0, THUMBNAIL_OUTLINE_THICKNESS);
            }
            else
            {
                drawList->AddRect(positionMin, positionMax, outlineColor, 0.f, 0, THUMBNAIL_OUTLINE_THICKNESS);
            }

            if(ImGui::InvisibleButton(uniformName.c_str(), EMPTY_THUMBNAIL_SIZE))
            {
                TryApplyingTextureFor(material, uniformName, slot);
            }
        }

        void MaterialInspector::TryApplyingTextureFor(Glacirer::Rendering::Material& material, const std::string& name, const unsigned int slot)
        {
            constexpr const wchar_t* TEXTURES_FILTER = L"All supported images\0*.png;*.jpg;*.jpeg;*.tga;*.bmp;*.psd;*.gif;*.hdr\0"
                L"PNG (*.png)\0*.png\0"
                L"JPG (*.jpg)\0*.jpg\0"
                L"JPEG (*.jpeg)\0*.jpeg\0"
                L"TGA (*.tga)\0*.tga\0"
                L"BMP (*.bmp)\0*.bmp\0"
                L"PSD (*.psd)\0*.psd\0"
                L"GIF (*.gif)\0*.gif\0"
                L"HDR (*.hdr)\0*.hdr\0";

            std::string texturePath;
            if(!Util::FileDialog::OpenFile(TEXTURES_FILTER, texturePath))
            {
                return;
            }

            std::hash<std::string> hasher;
            std::string hashedPath = std::to_string(static_cast<int>(hasher(texturePath)));

            Glacirer::Rendering::TextureSettings settings{};
            std::shared_ptr<Glacirer::Rendering::Texture> texture = Glacirer::Resources::ResourceManager::GetOrLoadTexture(texturePath, hashedPath, settings, false);

            material.SetTexture(name, texture, slot);
        }

        void MaterialInspector::RenderBoolProperties(Glacirer::Rendering::Material& material)
        {
            const auto& materialBoolProperties = material.GetAllBoolProperties();
            std::map<std::string, bool> inspectorBoolProperties{};

            for (const auto& boolPropertyPair : materialBoolProperties)
            {
                std::string boolName = boolPropertyPair.first;
                
                inspectorBoolProperties[boolName] = boolPropertyPair.second;
                ImGui::Checkbox(boolName.c_str(), &inspectorBoolProperties[boolName]);
            }

            for (const auto& inspectorBoolPropertyPair : inspectorBoolProperties)
            {
                material.SetBool(inspectorBoolPropertyPair.first, inspectorBoolPropertyPair.second);
            }
        }

        void MaterialInspector::RenderFloatProperties(Glacirer::Rendering::Material& material)
        {
            const auto& materialFloatProperties = material.GetAllFloatProperties();
            std::map<std::string, float> inspectorFloatProperties{};

            for (const auto& floatPropertyPair : materialFloatProperties)
            {
                std::string floatName = floatPropertyPair.first;

                inspectorFloatProperties[floatName] = floatPropertyPair.second;
                ImGui::DragFloat(floatName.c_str(), &inspectorFloatProperties[floatName]);
            }

            for (const auto& inspectorFloatPropertyPair : inspectorFloatProperties)
            {
                material.SetFloat(inspectorFloatPropertyPair.first, inspectorFloatPropertyPair.second);
            }
        }

        void MaterialInspector::RenderIntProperties(Glacirer::Rendering::Material& material)
        {
            const auto& materialIntProperties = material.GetAllIntProperties();
            std::map<std::string, int> inspectorIntProperties{};

            for (const auto& intPropertyPair : materialIntProperties)
            {
                std::string intName = intPropertyPair.first;

                inspectorIntProperties[intName] = intPropertyPair.second;
                ImGui::DragInt(intName.c_str(), &inspectorIntProperties[intName]);
            }

            for (const auto& inspectorIntPropertyPair : inspectorIntProperties)
            {
                material.SetInt(inspectorIntPropertyPair.first, inspectorIntPropertyPair.second);
            }
        }
    }
}
