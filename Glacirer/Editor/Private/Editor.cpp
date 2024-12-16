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
#include "Sandbox/SandboxSceneSpawner.h"

namespace GlacirerEditor
{
    void Editor::Initialize()
    {
        m_Engine.Initialize("Glacirer Engine");

        InitializeImGUI();
    }

    void Editor::Setup()
    {
        m_Engine.Setup();

        Sandbox::SandboxSceneSpawner::Spawn(m_Engine.GetWorld());
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
        m_Engine.Update();

        UpdateSelectedGameObject(m_Engine.GetWorld());
        UpdateShortcuts();
    }

    void Editor::Render()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        m_Engine.Render();

        RenderGUI(m_Engine.GetWorld());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Editor::Shutdown()
    {
        m_Engine.Shutdown();
    }

    void Editor::RenderGUI(Glacirer::World& world)
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

    void Editor::UpdateSelectedGameObject(const Glacirer::World& world)
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

    void Editor::SelectGameObject(int index, const Glacirer::World& world)
    {
        std::shared_ptr<Glacirer::GameObject> selectedGameObject = world.GetGameObjectAt(index);

        std::weak_ptr<Glacirer::MeshComponent> meshComponentWeak = selectedGameObject->GetComponent<Glacirer::MeshComponent>();
        std::shared_ptr<Glacirer::MeshComponent> meshComponent = meshComponentWeak.lock();

        if(meshComponent)
        {
            meshComponent->SetIsOutlined(true);
        }
    }

    void Editor::DeselectGameObject(int index, const Glacirer::World& world)
    {
        std::shared_ptr<Glacirer::GameObject> selectedGameObject = world.GetGameObjectAt(index);

        std::weak_ptr<Glacirer::MeshComponent> meshComponentWeak = selectedGameObject->GetComponent<Glacirer::MeshComponent>();
        std::shared_ptr<Glacirer::MeshComponent> meshComponent = meshComponentWeak.lock();

        if(meshComponent)
        {
            meshComponent->SetIsOutlined(false);
        }
    }

    void Editor::UpdateShortcuts()
    {
        if(Glacirer::Input::GetKey(GLFW_KEY_LEFT_CONTROL) && Glacirer::Input::GetKeyDown(GLFW_KEY_SPACE))
        {
            bShowPanelsEnabled = !bShowPanelsEnabled;
        }

        if (Glacirer::Input::GetKeyDown(GLFW_KEY_DELETE) && m_MainPanel.HasAnyGameObjectSelected())
        {
            DeleteSelectedGameObject();
        }
    }

    void Editor::DeleteSelectedGameObject()
    {
        int selectedIndex = m_MainPanel.GetCurrentSelectedGameObjectIndex();

        if(selectedIndex < 0)
        {
            return;
        }

        const Glacirer::World& world = m_Engine.GetWorld();
        std::shared_ptr<Glacirer::GameObject> selectedGameObject = world.GetGameObjectAt(selectedIndex);
        selectedGameObject->Destroy();

        m_MainPanel.ResetSelection();
        m_SelectedGameObjectIndex = -1;
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

