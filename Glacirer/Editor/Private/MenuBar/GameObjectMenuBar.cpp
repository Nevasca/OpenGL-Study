#include "MenuBar/GameObjectMenuBar.h"

#include <imgui/imgui.h>
#include "World.h"
#include "Basics/Objects/Camera.h"
#include "Basics/Objects/Cube.h"
#include "Basics/Objects/DirectionalLight.h"
#include "Basics/Objects/PointLight.h"
#include "Basics/Objects/Quad.h"
#include "Basics/Objects/Sphere.h"
#include "Basics/Objects/SpotLight.h"
#include "GameObject/GameObject.h"

namespace GlacirerEditor
{
    namespace MenuBar
    {
        void GameObjectMenuBar::RenderGUI(Glacirer::World& world)
        {
            if(!ImGui::BeginMenu("GameObject"))
            {
                return;
            }

            ImGui::SeparatorText("Add new");
        
            if(ImGui::MenuItem("Empty"))
            {
                world.Spawn<Glacirer::GameObject>();
            }

            RenderShapesSubmenu(world);
            RenderLightSubmenu(world);
            RenderCameraSubmenu(world);

            ImGui::EndMenu();
        }

        void GameObjectMenuBar::RenderShapesSubmenu(Glacirer::World& world)
        {
            if (!ImGui::BeginMenu("Shapes"))
            {
                return;
            }

            if(ImGui::MenuItem("Cube"))
            {
                std::shared_ptr<Glacirer::Cube> shape = world.Spawn<Glacirer::Cube>();
                shape->SetName("Cube" + std::to_string(shape->GetId()));
            }

            if(ImGui::MenuItem("Sphere"))
            {
                std::shared_ptr<Glacirer::Sphere> shape = world.Spawn<Glacirer::Sphere>();
                shape->SetName("Sphere" + std::to_string(shape->GetId()));
            }

            if(ImGui::MenuItem("Quad"))
            {
                std::shared_ptr<Glacirer::Quad> shape = world.Spawn<Glacirer::Quad>();
                shape->SetName("Quad" + std::to_string(shape->GetId()));
            }
            
            ImGui::EndMenu();
        }

        void GameObjectMenuBar::RenderLightSubmenu(Glacirer::World& world)
        {
            if (!ImGui::BeginMenu("Light"))
            {
                return;
            }

            if(ImGui::MenuItem("Directional Light"))
            {
                std::shared_ptr<Glacirer::DirectionalLight> light = world.Spawn<Glacirer::DirectionalLight>();
                light->SetName("DirectionalLight" + std::to_string(light->GetId()));
            }

            if(ImGui::MenuItem("Point Light"))
            {
                std::shared_ptr<Glacirer::PointLight> light = world.Spawn<Glacirer::PointLight>();
                light->SetName("PointLight" + std::to_string(light->GetId()));
            }

            if(ImGui::MenuItem("Spot Light"))
            {
                std::shared_ptr<Glacirer::SpotLight> light = world.Spawn<Glacirer::SpotLight>();
                light->SetName("SpotLight" + std::to_string(light->GetId()));
            }
            
            ImGui::EndMenu();
        }

        void GameObjectMenuBar::RenderCameraSubmenu(Glacirer::World& world)
        {
            if(ImGui::MenuItem("Camera"))
            {
                std::shared_ptr<Glacirer::Camera> camera = world.Spawn<Glacirer::Camera>();
                camera->SetName("Camera" + std::to_string(camera->GetId()));
            }
        }
    }
}
