#include "WorldInspector.h"

#include "core/World.h"
#include "core/Rendering/RenderSystem.h"
#include <imgui/imgui.h>

namespace Editor
{
    namespace Inspector
    {
        void WorldInspector::RenderGUI(World& world)
        {
            if(ImGui::CollapsingHeader("Lighting", ImGuiTreeNodeFlags_DefaultOpen))
            {
                RenderLightingProperties(world);
            }

            if(ImGui::CollapsingHeader("Rendering", ImGuiTreeNodeFlags_DefaultOpen))
            {
                RenderRenderingProperties(world);
            }
        }

        void WorldInspector::RenderLightingProperties(World& world)
        {
            RenderSystem& renderSystem = world.GetRenderSystem();

            glm::vec3 ambientLightColor = renderSystem.GetAmbientLightColor();
            ImGui::ColorEdit3("Ambient Light", &ambientLightColor.r);
            
            renderSystem.SetAmbientLightColor(ambientLightColor);
        }

        void WorldInspector::RenderRenderingProperties(World& world)
        {
            RenderSystem& renderSystem = world.GetRenderSystem();

            glm::vec4 clearColor = renderSystem.GetClearColor();
            ImGui::ColorEdit4("Clear Color", &clearColor.r);

            renderSystem.SetClearColor(clearColor);

            Rendering::Device& device = renderSystem.GetDevice();

            bool bIsFaceCullingEnabled = device.IsFaceCullingEnabled();
            ImGui::Checkbox("Face culling", &bIsFaceCullingEnabled);

            if(bIsFaceCullingEnabled != device.IsFaceCullingEnabled())
            {
                if(bIsFaceCullingEnabled)
                {
                    device.EnableFaceCulling();
                }
                else
                {
                    device.DisableFaceCulling();
                }
            }
        }
    }
}
