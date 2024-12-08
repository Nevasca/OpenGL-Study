#include "Editor.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Input.h"
#include "World.h"
#include "GameObject/GameObject.h"
#include "Basics/Components/MeshComponent.h"

namespace Editor
{
    void Editor::Initialize()
    {
        m_Engine.Initialize("Glacirer Engine");

        InitializeImGUI();
    }

    void Editor::Setup()
    {
        m_Engine.Setup();
    }

    bool Editor::IsInitialized() const
    {
        return m_Engine.IsInitialized();
    }

    bool Editor::ShouldClose() const
    {
        return m_Engine.ShouldClose();
    }

    void Editor::Update()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        m_Engine.Update();

        UpdateSelectedGameObject(m_Engine.GetWorld());
        UpdateShortcuts();
    }

    void Editor::Render()
    {
        m_Engine.Render();

        RenderGUI(m_Engine.GetWorld());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Editor::RenderGUI(World& world)
    {
        m_MainMenuBar.RenderGUI(world);

        if(!bShowPanelsEnabled)
        {
            return;
        }

        m_MainPanel.RenderGUI(world);
        m_ResourcesPanel.RenderGUI();
        m_StatisticsWindow.RenderGUI();
    }

    void Editor::UpdateSelectedGameObject(const World& world)
    {
        if(m_MainPanel.HasAnyGameObjectSelected())
        {
            int selectedIndex = m_MainPanel.GetCurrentSelectedGameObjectIndex();

            if(m_SelectedGameObjectIndex >= 0 && m_SelectedGameObjectIndex != selectedIndex)
            {
                DeselectGameObject(m_SelectedGameObjectIndex, world);
            }
            
            m_SelectedGameObjectIndex = selectedIndex; 
            SelectGameObject(m_SelectedGameObjectIndex, world);
        }
        else if(m_SelectedGameObjectIndex >= 0)
        {
            DeselectGameObject(m_SelectedGameObjectIndex, world);
            m_SelectedGameObjectIndex = -1;
        }
    }

    void Editor::SelectGameObject(int index, const World& world)
    {
        std::shared_ptr<GameObject> selectedGameObject = world.GetGameObjectAt(index);
        std::shared_ptr<MeshComponent> meshComponent = selectedGameObject->GetComponent<MeshComponent>();

        if(meshComponent)
        {
            meshComponent->SetIsOutlined(true);
        }
    }

    void Editor::DeselectGameObject(int index, const World& world)
    {
        std::shared_ptr<GameObject> selectedGameObject = world.GetGameObjectAt(index);
        std::shared_ptr<MeshComponent> meshComponent = selectedGameObject->GetComponent<MeshComponent>();

        if(meshComponent)
        {
            meshComponent->SetIsOutlined(false);
        }
    }

    void Editor::UpdateShortcuts()
    {
        if(Input::GetKey(GLFW_KEY_LEFT_CONTROL) && Input::GetKeyDown(GLFW_KEY_SPACE))
        {
            bShowPanelsEnabled = !bShowPanelsEnabled;
        }
    }

    void Editor::InitializeImGUI()
    {
        GLFWwindow* window = m_Engine.GetWindow();

        ImGui::CreateContext();
        
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(Glacirer::Engine::GetGLSLVersion());
        ImGui::StyleColorsDark();
    }
}

