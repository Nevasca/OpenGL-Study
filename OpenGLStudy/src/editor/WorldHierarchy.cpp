#include "WorldHierarchy.h"

#include <string>
#include <imgui/imgui.h>

#include "core/World.h"
#include "core/GameObject/GameObject.h"

namespace Editor
{
    void WorldHierarchy::RenderGUI(World& world)
    {
        ImGui::Begin("Hierarchy");

        std::vector<std::shared_ptr<GameObject>>& gameObjects = world.GetAllGameObjects();

        for(int i = 0; i < static_cast<int>(gameObjects.size()); i++)
        {
            ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth;

            if(m_SelectedIndex == i)
            {
                nodeFlags |= ImGuiTreeNodeFlags_Selected;
            }
            
            ImGui::TreeNodeEx((void*)(intptr_t)i, nodeFlags, "%s", gameObjects[i]->GetName().c_str());

            if(ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            {
                m_SelectedIndex = i;
            }
        }
        
        ImGui::End();
    }
}
