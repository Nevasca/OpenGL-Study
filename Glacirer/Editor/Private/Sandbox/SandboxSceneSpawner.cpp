 #include "Sandbox/SandboxSceneSpawner.h"

#include <memory>

#include "Resources/ResourceManager.h"
#include "World.h"
#include "Basics/Objects/FlyingCamera.h"
#include "glm/vec3.hpp"
#include "Rendering/Material.h"
#include "Basics/Objects/Cube.h"
#include "Basics/Objects/DirectionalLight.h"
#include "Basics/Objects/Model.h"
#include "Basics/Objects/PointLight.h"
#include "Basics/Objects/PostProcessing.h"
#include "Basics/Objects/Quad.h"
#include "Basics/Objects/Skybox.h"
#include "Basics/Objects/Sphere.h"
#include "Basics/Objects/SpotLight.h"

namespace GlacirerEditor
{
    namespace Sandbox
    {
        std::string SandboxSceneSpawner::SANDBOX_RESOURCES_PATH = "EditorData/Sandbox/";
        
        void SandboxSceneSpawner::Spawn(Glacirer::World& world)
        {
            glm::vec3 cameraInitialPosition(-12.f, 11.f, 12.f);
            glm::vec3 cameraInitialRotation(-25.f, -47.f, 0.f);
            std::shared_ptr<Glacirer::FlyingCamera> camera = world.Spawn<Glacirer::FlyingCamera>(cameraInitialPosition, cameraInitialRotation);
            camera->DisablePilotMode();
            camera->SetName("Camera");

            SpawnCrates(world);

            auto cube = world.Spawn<Glacirer::Cube>(glm::vec3(0.f, 5.f, 0.f));
            cube->SetName("CubePigeon");
            auto anotherMaterial = Glacirer::Resources::ResourceManager::CreateMaterial("AnotherMaterial");
            anotherMaterial->SetColor("u_Color", glm::vec4(0.f));

            Glacirer::Rendering::TextureSettings pigeonTextureSettings{false};
            pigeonTextureSettings.GenerateMipmap = true;
            anotherMaterial->SetTexture("u_Diffuse", Glacirer::Resources::ResourceManager::LoadTexture(SANDBOX_RESOURCES_PATH + "Textures/FancyPigeon.png", "Pigeon", pigeonTextureSettings), 0);
            cube->SetMaterial(anotherMaterial);

            SpawnBridge(world);
            SpawnWarrior(world);

            SpawnTransparentObjects(world);

            auto sphere = world.Spawn<Glacirer::Sphere>(glm::vec3(-4.f, 1.f, 2.f));
            auto sphereMaterial = Glacirer::Resources::ResourceManager::CreateMaterial("M_Sphere");
            sphereMaterial->SetFloat("u_ReflectionValue", 1.f);
            sphereMaterial->SetColor("u_Color", glm::vec4{0.1f});
            sphere->SetName("Sphere");
            sphere->SetMaterial(sphereMaterial);

            SpawnFloor(world);
            SpawnLights(world, *camera);

            auto postProcessing = world.Spawn<Glacirer::PostProcessing>();
            postProcessing->SetName("PostProcessing");

            auto skybox = world.Spawn<Glacirer::Skybox>();
            skybox->SetName("Skybox");
            skybox->SetDefaultSky();
        }

        void SandboxSceneSpawner::SpawnCrates(Glacirer::World& world)
        {
            std::shared_ptr<Glacirer::Rendering::Material> crateMaterial = Glacirer::Resources::ResourceManager::CreateMaterial("M_Crate");
            crateMaterial->SetColor("u_Color", glm::vec4(0.f)); // When using a texture, we need to set default color to black
            crateMaterial->SetTexture("u_Diffuse", Glacirer::Resources::ResourceManager::LoadTexture(SANDBOX_RESOURCES_PATH + "Textures/Container_Diff.png", "Container_Diffuse", Glacirer::Rendering::TextureSettings{false}), 0);
            crateMaterial->SetTexture("u_Specular", Glacirer::Resources::ResourceManager::LoadTexture(SANDBOX_RESOURCES_PATH + "Textures/Container_Spec.png", "Container_Specular", Glacirer::Rendering::TextureSettings{false, false}), 1);
        
            int crateIndex = 0;
            for(int x = 0; x < 3; x++)
            {
                for(int z = 0; z < 3; z++)
                {
                    glm::vec3 position{static_cast<float>(x) * 2.f, 0.f, static_cast<float>(z) * 2.f};
                    auto crate = world.Spawn<Glacirer::Cube>(position);
                    crate->SetName("Crate" + std::to_string(crateIndex));
                    crate->SetMaterial(crateMaterial);

                    crateIndex++;
                }
            }
        }

        void SandboxSceneSpawner::SpawnBridge(Glacirer::World& world)
        {
            auto bridgeMaterial = Glacirer::Resources::ResourceManager::CreateMaterial("M_Bridge");
            bridgeMaterial->SetColor("u_Color", glm::vec4(0.f));
            bridgeMaterial->SetTexture("u_Diffuse", Glacirer::Resources::ResourceManager::LoadTexture(SANDBOX_RESOURCES_PATH + "Textures/Atlas04_Diff.png", "T_Bridge_Diffuse", Glacirer::Rendering::TextureSettings{false}), 0);

            auto bridgeModel = Glacirer::Resources::ResourceManager::LoadModel(SANDBOX_RESOURCES_PATH + "Models/Bridge.fbx", "Bridge");

            auto bridge = world.Spawn<Glacirer::Model>(glm::vec3(0.f, 0.f, -20.f));
            bridge->Setup(bridgeModel, bridgeMaterial);
            bridge->SetName("Bridge");
        }

        void SandboxSceneSpawner::SpawnWarrior(Glacirer::World& world)
        {
            auto warriorMaterial = Glacirer::Resources::ResourceManager::CreateMaterial("M_Liz");
            warriorMaterial->SetColor("u_Color", glm::vec4(0.f));
            warriorMaterial->SetTexture("u_Diffuse", Glacirer::Resources::ResourceManager::LoadTexture(SANDBOX_RESOURCES_PATH + "Textures/liz/T_Liz_Diffuse.png", "T_Liz_Diffuse", Glacirer::Rendering::TextureSettings{false}), 0);
            warriorMaterial->SetTexture("u_Specular", Glacirer::Resources::ResourceManager::LoadTexture(SANDBOX_RESOURCES_PATH + "Textures/liz/T_Liz_Specular.png", "T_Liz_Specular", Glacirer::Rendering::TextureSettings{false}), 1);
        
            auto warriorModel = Glacirer::Resources::ResourceManager::LoadModel(SANDBOX_RESOURCES_PATH + "Models/PigeonsAttack_Liz.fbx", "Liz");

            glm::vec3 spawnPosition{0.f, -0.5f, -4.f};
            glm::vec3 spawnRotation{0.f, -31.f, 0.f};
            glm::vec3 spawnScale{0.25f};

            auto warrior = world.Spawn<Glacirer::Model>(spawnPosition, spawnRotation, spawnScale);
            warrior->Setup(warriorModel, warriorMaterial);
            warrior->SetName("Warrior");
        }

        void SandboxSceneSpawner::SpawnTransparentObjects(Glacirer::World& world)
        {
            auto flowerMaterial = Glacirer::Resources::ResourceManager::CreateMaterial("M_Flower");
            flowerMaterial->SetColor("u_Color", glm::vec4(0.f));
            flowerMaterial->SetTexture("u_Diffuse", Glacirer::Resources::ResourceManager::LoadTexture(SANDBOX_RESOURCES_PATH + "Textures/Flower.png", "T_Flower", Glacirer::Rendering::TextureSettings{true}), 0);
            flowerMaterial->SetRenderingMode(Glacirer::Rendering::MaterialRenderingMode::AlphaCutout);
        
            auto flowerQuad = world.Spawn<Glacirer::Quad>(glm::vec3(-2.f, 0.f, -2.f), glm::vec3{0.f, -90.f, 0.f});
            flowerQuad->SetName("Flower");
            flowerQuad->SetMaterial(flowerMaterial);

            auto windowMaterial = Glacirer::Resources::ResourceManager::CreateMaterial("M_Window");
            windowMaterial->SetColor("u_Color", glm::vec4(0.f));
            windowMaterial->SetTexture("u_Diffuse", Glacirer::Resources::ResourceManager::LoadTexture(SANDBOX_RESOURCES_PATH + "Textures/TransparentWindow.png", "T_Window", Glacirer::Rendering::TextureSettings{true}), 0);
            windowMaterial->SetRenderingMode(Glacirer::Rendering::MaterialRenderingMode::Transparent);

            glm::vec3 windowRotation{0.f, -90.f, 0.f};

            for(int i = 0; i < 5; i++)
            {
                glm::vec3 windowPosition(static_cast<float>(i) * 2.f, 0.f, -2.f);

                auto windowQuad = world.Spawn<Glacirer::Quad>(windowPosition, windowRotation);
                windowQuad->SetName("Window" + std::to_string(i));
                windowQuad->SetMaterial(windowMaterial);
            }
        }

        void SandboxSceneSpawner::SpawnFloor(Glacirer::World& world)
        {
            glm::vec3 floorPosition{4.f, -0.75f, 4.f};
            glm::vec3 floorRotation{0.f};
            glm::vec3 floorScale{44.f, 0.5f, 44.f};
            std::shared_ptr<Glacirer::Cube> floor = world.Spawn<Glacirer::Cube>(floorPosition, floorRotation, floorScale);
            floor->SetName("Floor");
        }

        void SandboxSceneSpawner::SpawnLights(Glacirer::World& world, Glacirer::GameObject& camera)
        {
            int lightIndex = 0;
            for(int x = 0; x < 1; x++)
            {
                for(int z = 0; z < 2; z++)
                {
                    glm::vec3 position{static_cast<float>(x) * 2.f, 3.f, static_cast<float>(z) * 2.f};
                    auto light = world.Spawn<Glacirer::PointLight>(position);
                    light->SetName("PointLight" + std::to_string(lightIndex));
                    light->SetColor(glm::vec3(1.f, 1.f, 1.f));

                    lightIndex++;
                }
            }

            std::shared_ptr<Glacirer::DirectionalLight> mainDirectionalLight = world.Spawn<Glacirer::DirectionalLight>();
            mainDirectionalLight->SetPosition(glm::vec3(-6.f, 15.f, 4.f));
            mainDirectionalLight->SetName("MainDirectionalLight");
            mainDirectionalLight->SetRotation(glm::vec3(-45.f, -55.f, 0.f));
            mainDirectionalLight->SetColor(glm::vec3(1.f, 0.82f, 0.635f));
            mainDirectionalLight->SetIntensity(0.3f);

            std::shared_ptr<Glacirer::DirectionalLight> secondaryDirectionalLight = world.Spawn<Glacirer::DirectionalLight>();
            secondaryDirectionalLight->SetPosition(glm::vec3(-6.f, 15.f, 4.f));
            secondaryDirectionalLight->SetName("SecondaryDirectionalLight");
            secondaryDirectionalLight->SetRotation(glm::vec3(-45.f, -55.f, 0.f));
            secondaryDirectionalLight->SetColor(glm::vec3(0.82f, 0.875f, 1.f));
            secondaryDirectionalLight->SetIntensity(0.3f);
            secondaryDirectionalLight->SetCastShadowEnabled(false);

            std::shared_ptr<Glacirer::PointLight> pointLight = world.Spawn<Glacirer::PointLight>(glm::vec3(-1.f, 0.4f, 10.f));
            pointLight->SetName("PointLight" + std::to_string(lightIndex++));
            pointLight->SetColor(glm::vec3(1.f, 0.f, 0.f));
            pointLight->SetRange(60.f);

            std::shared_ptr<Glacirer::SpotLight> spotLight = world.Spawn<Glacirer::SpotLight>(glm::vec3{2.2f, 4.5f, 6.5f}, glm::vec3{-41.f, 37.f, 0.f});
            spotLight->SetName("Spotlight");
            spotLight->SetRange(59.f);
            spotLight->SetInnerCutoffDegrees(22.f);
            spotLight->SetOuterCutoffDegrees(36.5f);
        }
    }
}