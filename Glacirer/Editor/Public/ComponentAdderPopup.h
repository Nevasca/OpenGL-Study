#pragma once

namespace Glacirer
{
    class GameObject;
}

namespace GlacirerEditor
{
    class ComponentAdderPopup
    {
    public:
        void OpenPopup();
        void RenderGUI(Glacirer::GameObject& selectedGameObject);

    private:
        void RenderRenderingSubmenu(Glacirer::GameObject& selectedGameObject);
    };
}
