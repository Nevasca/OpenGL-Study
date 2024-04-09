#include "EngineEditor.h"

#include <memory>

#include "core/World.h"
#include "imgui/imgui.h"

namespace Editor
{
    void EngineEditor::Initialize()
    {
        
    }

    void EngineEditor::RenderGUI(World& world)
    {
        m_Hierarchy.RenderGUI(world);

        ImGui::Begin("Inspector");

        if(m_Hierarchy.HasAnyGameObjectSelected())
        {
            int selectedIndex = m_Hierarchy.GetCurrentSelectedGameObjectIndex();
            std::shared_ptr<GameObject> selectedObject = world.GetGameObjectAt(selectedIndex);

            m_GameObjectInspector.RenderGUI(*selectedObject);
        }

        ImGui::End();
    }
}

