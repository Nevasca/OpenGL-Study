#pragma once
#include "GameObjectInspector.h"
#include "WorldHierarchy.h"

class World;

namespace Editor
{
    class EngineEditor
    {
    public:

        void Initialize();
        void RenderGUI(World& world);

    private:

        WorldHierarchy m_Hierarchy{};
        GameObjectInspector m_GameObjectInspector{};
    };
}
