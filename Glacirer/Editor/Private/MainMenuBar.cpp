#include "MainMenuBar.h"

#include <imgui/imgui.h>

#include "Resources/ResourceManager.h"
#include "World.h"
#include "Rendering/RenderSystem.h"

namespace GlacirerEditor
{
    void MainMenuBar::RenderGUI(Glacirer::World& world)
    {
        if(!ImGui::BeginMainMenuBar())
        {
            return;
        }

        RenderViewportMenu(world);

        ImGui::EndMainMenuBar();
    }

    void MainMenuBar::RenderViewportMenu(Glacirer::World& world)
    {
        if(!ImGui::BeginMenu("Viewport"))
        {
            return;
        }

        Glacirer::Rendering::RenderSystem& renderSystem = world.GetRenderSystem();

        ImGui::SeparatorText("Visualizations");
        
        if(ImGui::MenuItem("Lit"))
        {
            renderSystem.SetOverrideShader(nullptr);
        }

        if(ImGui::MenuItem("Unlit"))
        {
            std::string unlitShaderFilePath = Glacirer::Resources::ResourceManager::RESOURCES_PATH + "Shaders/DefaultUnlit.glsl";
            std::shared_ptr<Glacirer::Rendering::Shader> unlitShader = Glacirer::Resources::ResourceManager::GetOrLoadShader(unlitShaderFilePath, "Unlit");

            renderSystem.SetOverrideShader(unlitShader);
        }

        if(ImGui::MenuItem("Linear Depth"))
        {
            std::string depthBufferShaderFilePath = Glacirer::Resources::ResourceManager::RESOURCES_PATH + "Shaders/visualizers/DepthBufferVisualizer.glsl";
            std::shared_ptr<Glacirer::Rendering::Shader> depthBufferShader = Glacirer::Resources::ResourceManager::GetOrLoadShader(depthBufferShaderFilePath, "DepthBufferVisualizer");

            renderSystem.SetOverrideShader(depthBufferShader);
        }

        ImGui::EndMenu();
    }
}
