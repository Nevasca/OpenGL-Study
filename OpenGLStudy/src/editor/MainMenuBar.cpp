#include "MainMenuBar.h"

#include <imgui/imgui.h>

#include "core/ResourceManager.h"
#include "core/World.h"
#include "core/Rendering/RenderSystem.h"

namespace Editor
{
    void MainMenuBar::RenderGUI(World& world)
    {
        if(!ImGui::BeginMainMenuBar())
        {
            return;
        }

        RenderViewportMenu(world);

        ImGui::EndMainMenuBar();
    }

    void MainMenuBar::RenderViewportMenu(World& world)
    {
        if(!ImGui::BeginMenu("Viewport"))
        {
            return;
        }

        RenderSystem& renderSystem = world.GetRenderSystem();

        ImGui::SeparatorText("Visualizations");
        
        if(ImGui::MenuItem("Lit"))
        {
            renderSystem.SetOverrideShader(nullptr);
        }

        if(ImGui::MenuItem("Unlit"))
        {
            std::string unlitShaderFilePath = "res/core/shaders/DefaultUnlit.glsl";
            std::shared_ptr<Shader> unlitShader = ResourceManager::GetOrLoadShader(unlitShaderFilePath, "Unlit");

            renderSystem.SetOverrideShader(unlitShader);
        }

        if(ImGui::MenuItem("Linear Depth"))
        {
            std::string depthBufferShaderFilePath = "res/core/shaders/visualizers/DepthBufferVisualizer.glsl";
            std::shared_ptr<Shader> depthBufferShader = ResourceManager::GetOrLoadShader(depthBufferShaderFilePath, "DepthBufferVisualizer");

            renderSystem.SetOverrideShader(depthBufferShader);
        }

        ImGui::EndMenu();
    }
}
