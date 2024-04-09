#pragma once

class GameObject;

namespace Editor
{
    class GameObjectInspector
    {
    public:

        void RenderGUI(GameObject& gameObject);
    };
}
