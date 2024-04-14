#include "MaterialInspector.h"

#include <imgui/imgui.h>

#include "core/Rendering/Material.h"
#include "core/Rendering/Shader.h"
#include "core/Rendering/Texture.h"

namespace Editor
{
    namespace Inspector
    {
        void MaterialInspector::RenderGUI(Material& material)
        {
            ImGui::SeparatorText(material.GetName().c_str());

            const std::shared_ptr<Shader>& shader = material.GetShader();
            std::string shaderName = shader ? shader->GetName() : "None";
            ImGui::Text("Shader: %s", shaderName.c_str());

            RenderColorProperties(material);
            RenderTextureProperties(material);
        }

        void MaterialInspector::RenderColorProperties(Material& material)
        {
            const auto& materialColorProperties = material.GetAllColorProperties();
            std::map<std::string, glm::vec4> inspectorColorProperties{};

            for (const auto& colorPropertyPair : materialColorProperties)
            {
                std::string colorName = colorPropertyPair.first;
                
                inspectorColorProperties[colorName] = colorPropertyPair.second;
                ImGui::ColorEdit3(colorName.c_str(), &inspectorColorProperties[colorName].r);
            }

            for (const auto& editedColorPropertyPair : inspectorColorProperties)
            {
                material.SetColor(editedColorPropertyPair.first, editedColorPropertyPair.second);
            }
        }

        void MaterialInspector::RenderTextureProperties(Material& material)
        {
            const auto& materialTextureProperties = material.GetAllTextureProperties();
            
            for (const auto& texturePropertyPair : materialTextureProperties)
            {
                auto& textureProperty = texturePropertyPair.second;
                ImGui::Text("%s: %s", texturePropertyPair.first.c_str(), textureProperty.Texture->GetName().c_str());

                ImVec2 uv0{0, 0};
                ImVec2 uv1{1, 1};

                // If we flipped the texture on load (OpenGL generally requires that), we need to invert uv (top left v0 and top right v1)
                // to proper display it on inspectors
                if(textureProperty.Texture->IsFlippedOnLoad())
                {
                    uv0.y = 1;
                    uv1.y = 0;
                }

                ImGui::Image((ImTextureID)textureProperty.Texture->GetRendererID(), ImVec2(100.f, 100.f), uv0, uv1);
            }
        }
    }
}
