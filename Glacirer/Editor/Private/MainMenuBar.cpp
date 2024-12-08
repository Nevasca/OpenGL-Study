#include "MainMenuBar.h"

#include <imgui/imgui.h>

#include "ResourceManager.h"
#include "World.h"
#include "Rendering/RenderSystem.h"

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
            std::string unlitShaderFilePath = ResourceManager::RESOURCES_PATH + "Shaders/DefaultUnlit.glsl";
            std::shared_ptr<Shader> unlitShader = ResourceManager::GetOrLoadShader(unlitShaderFilePath, "Unlit");

            renderSystem.SetOverrideShader(unlitShader);
        }

        if(ImGui::MenuItem("Linear Depth"))
        {
            std::string depthBufferShaderFilePath = ResourceManager::RESOURCES_PATH + "Shaders/visualizers/DepthBufferVisualizer.glsl";
            std::shared_ptr<Shader> depthBufferShader = ResourceManager::GetOrLoadShader(depthBufferShaderFilePath, "DepthBufferVisualizer");

            renderSystem.SetOverrideShader(depthBufferShader);
        }

        ImGui::EndMenu();
    }
}
