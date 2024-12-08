#pragma once

#include "imgui/imgui.h"

namespace Editor
{
    class StatisticsWindow
    {
        public:
            void RenderGUI();

        private:
            ImVec2 m_InitialPosition{380.f, 30.f};
    };
}
