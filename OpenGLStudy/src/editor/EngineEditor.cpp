#include "EngineEditor.h"

namespace Editor
{
    void EngineEditor::Initialize()
    { }

    void EngineEditor::RenderGUI(World& world)
    {
        m_MainPanel.RenderGUI(world);
        m_ResourcesPanel.RenderGUI();
    }
}

