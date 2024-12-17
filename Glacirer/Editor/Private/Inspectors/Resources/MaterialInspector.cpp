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
                auto& textureProperty = texturePropertyPair.second;
                std::shared_ptr<Glacirer::Rendering::Texture> texture = textureProperty.Texture;

                // TODO: refactor and implement showing empty square when no texture
                if(texture)
                {
                    ImGui::Text("%s: %s", texturePropertyPair.first.c_str(), texture->GetName().c_str());

                    ImVec2 uv0{0, 0};
                    ImVec2 uv1{1, 1};

                    // If we flipped the texture on load (OpenGL generally requires that), we need to invert uv (top left v0 and top right v1)
                    // to proper display it on inspectors
                    if(texture->IsFlippedOnLoad())
                    {
                        uv0.y = 1;
                        uv1.y = 0;
                    }

                    ImGui::Image((ImTextureID)texture->GetRendererID(), ImVec2(100.f, 100.f), uv0, uv1);
                    if (ImGui::IsItemClicked())
                    {
                        TryApplyingTextureFor(material, texturePropertyPair.first, textureProperty.Slot);
                    }
                }
                else
                {
                    ImGui::Text("%s", texturePropertyPair.first.c_str());
                    ImGui::SameLine();
                    if(ImGui::Button("Select Texture"))
                    {
                        TryApplyingTextureFor(material, texturePropertyPair.first, textureProperty.Slot);
                    }
                }
            }
        }

        void MaterialInspector::TryApplyingTextureFor(Glacirer::Rendering::Material& material, const std::string& name, const unsigned int slot)
        {
            std::string texturePath;
            // TODO: check supported formats on stb image lib
            const wchar_t* filter = L"Textures\0*.png;*.jpg;*.jpeg\0";

            if(Util::FileDialog::OpenFile(filter, texturePath))
            {
                static int test = 0;

                Glacirer::Rendering::TextureSettings settings{false};
                auto textureName = std::to_string(test++);
                auto texture = Glacirer::Resources::ResourceManager::LoadTexture(texturePath, textureName, settings, true);

                material.SetTexture(name, texture, slot);
            }
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
