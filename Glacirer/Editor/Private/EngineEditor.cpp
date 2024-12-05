#include "EngineEditor.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "core/Input.h"
#include "core/World.h"
#include "core/GameObject/GameObject.h"
#include "core/Basics/Components/MeshComponent.h"

namespace Editor
{
    void EngineEditor::Initialize()
    { }

    void EngineEditor::Update(float deltaTime, World& world)
    {
        UpdateSelectedGameObject(world);
        UpdateShortcuts();
    }

    void EngineEditor::RenderGUI(World& world)
    {
        m_MainMenuBar.RenderGUI(world);

        if(!bShowPanelsEnabled)
        {
            return;
        }

        m_MainPanel.RenderGUI(world);
        m_ResourcesPanel.RenderGUI();
    }

    void EngineEditor::UpdateSelectedGameObject(const World& world)
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

    void EngineEditor::SelectGameObject(int index, const World& world)
    {
        std::shared_ptr<GameObject> selectedGameObject = world.GetGameObjectAt(index);
        std::shared_ptr<MeshComponent> meshComponent = selectedGameObject->GetComponent<MeshComponent>();

        if(meshComponent)
        {
            meshComponent->SetIsOutlined(true);
        }
    }

    void EngineEditor::DeselectGameObject(int index, const World& world)
    {
        std::shared_ptr<GameObject> selectedGameObject = world.GetGameObjectAt(index);
        std::shared_ptr<MeshComponent> meshComponent = selectedGameObject->GetComponent<MeshComponent>();

        if(meshComponent)
        {
            meshComponent->SetIsOutlined(false);
        }
    }

    void EngineEditor::UpdateShortcuts()
    {
        if(Input::GetKey(GLFW_KEY_LEFT_CONTROL) && Input::GetKeyDown(GLFW_KEY_SPACE))
        {
            bShowPanelsEnabled = !bShowPanelsEnabled;
        }
    }
}

