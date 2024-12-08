#include "StatisticsWindow.h"

namespace Editor
{
    void StatisticsWindow::RenderGUI()
    {
        ImGuiIO& io = ImGui::GetIO();
        
        ImGui::SetNextWindowPos(m_InitialPosition, ImGuiCond_FirstUseEver);
        ImGui::Begin("Statistics");
        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }
}
