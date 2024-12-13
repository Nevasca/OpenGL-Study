#include "MenuBar/GameObjectMenuBar.h"

#include <imgui/imgui.h>
#include "World.h"
#include "Basics/Objects/Camera.h"
#include "Basics/Objects/Cube.h"
#include "Basics/Objects/DirectionalLight.h"
#include "Basics/Objects/PointLight.h"
#include "Basics/Objects/PostProcessing.h"
#include "Basics/Objects/Quad.h"
#include "Basics/Objects/Skybox.h"
#include "Basics/Objects/Sphere.h"
#include "Basics/Objects/SpotLight.h"
#include "GameObject/GameObject.h"
#include "Basics/Objects/Model.h"
#include "Resources/ResourceManager.h"
#include "Util/FileDialog.h"

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
            RenderModelSubmenu(world);
            RenderLightSubmenu(world);
            RenderCameraSubmenu(world);
            RenderSkyboxSubmenu(world);
            RenderPostProcessingSubmenu(world);

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
        
        void GameObjectMenuBar::RenderSkyboxSubmenu(Glacirer::World& world)
        {
            if(ImGui::MenuItem("Skybox"))
            {
                std::shared_ptr<Glacirer::Skybox> skybox = world.Spawn<Glacirer::Skybox>();
                skybox->SetName("Skybox" + std::to_string(skybox->GetId()));
                skybox->SetDefaultSky();
            }
        }

        void GameObjectMenuBar::RenderPostProcessingSubmenu(Glacirer::World& world)
        {
            if(ImGui::MenuItem("PostProcessing"))
            {
                std::shared_ptr<Glacirer::PostProcessing> postProcessing = world.Spawn<Glacirer::PostProcessing>();
                postProcessing->SetName("PostProcessing" + std::to_string(postProcessing->GetId()));
            }
        }

        void GameObjectMenuBar::RenderModelSubmenu(Glacirer::World& world)
        {
            constexpr const wchar_t* MODELS_FILTER =
                L"All Supported Files\0*.3mf;*.dae;*.xml;*.bvh;*.3ds;*.ase;*.gltf;*.fbx;*.ply;*.dxf;*.ifc;*.iqm;*.nff;*.smd;*.vta;*.mdl;*.md2;*.md3;*.pk3;*.mdc;*.md5mesh;*.md5anim;*.md5camera;*.x;*.q3o;*.q3s;*.raw;*.ac;*.stl;*.irrmesh;*.off;*.obj;*.ter;*.hmp;*.mesh.xml;*.ogex;*.ms3d;*.lwo;*.lws;*.lxo;*.csm;*.cob;*.scn;*.xgl\0"
                L"3D Manufacturing Format (*.3mf)\0*.3mf\0"
                L"Collada (*.dae)\0*.dae\0"
                L"Biovision BVH (*.bvh)\0*.bvh\0"
                L"3D Studio Max 3DS (*.3ds)\0*.3ds\0"
                L"glTF (*.gltf)\0*.gltf\0"
                L"FBX (*.fbx)\0*.fbx\0"
                L"Stanford Polygon Library (*.ply)\0*.ply\0"
                L"AutoCAD DXF (*.dxf)\0*.dxf\0"
                L"Wavefront Object (*.obj)\0*.obj\0"
                L"Doom 3 (*.md5mesh)\0*.md5mesh\0"
                L"DirectX X (*.x)\0*.x\0"
                L"AC3D (*.ac)\0*.ac\0"
                L"Stereolithography (*.stl)\0*.stl\0"
                L"Terragen Terrain (*.ter)\0*.ter\0"
                L"LightWave Model (*.lwo)\0*.lwo\0"
                L"TrueSpace (*.cob)\0*.cob\0";
            
            if(!ImGui::MenuItem("Model"))
            {
                return;
            }

            std::string filePath{};
            if(!Util::FileDialog::OpenFile(MODELS_FILTER, filePath))
            {
                return;
            }

            std::hash<std::string> hasher;
            std::string hashedPath = std::to_string(static_cast<int>(hasher(filePath)));
            auto model = Glacirer::Resources::ResourceManager::GetOrLoadModel(filePath, hashedPath);

            auto modelGameObject = world.Spawn<Glacirer::Model>();
            modelGameObject->SetName("Model" + std::to_string(modelGameObject->GetId()));
            modelGameObject->Setup(model, Glacirer::Resources::ResourceManager::GetDefaultMaterial());
        }
    }
}
