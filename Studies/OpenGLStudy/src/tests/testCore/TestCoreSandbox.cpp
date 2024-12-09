#include "TestCoreSandbox.h"

#include <memory>

#include "core/ResourceManager.h"
#include "core/Basics/Objects/FlyingCamera.h"
#include "core/Basics/Objects/Cube.h"
#include "core/Basics/Objects/DirectionalLight.h"
#include "core/Basics/Components/DirectionalLightComponent.h"
#include "core/Basics/Objects/PointLight.h"
#include "core/Basics/Components/SpotLightComponent.h"
#include "core/Basics/Objects/Model.h"
#include "core/Basics/Objects/PostProcessing.h"
#include "core/Basics/Objects/Quad.h"
#include "core/Basics/Objects/Skybox.h"
#include "core/Basics/Objects/Sphere.h"
#include "core/Basics/Objects/SpotLight.h"
#include "core/Rendering/Material.h"

namespace tests
{
    TestCoreSandbox::TestCoreSandbox()
    {
        ResourceManager::LoadDefaultResources();

        m_World = std::make_unique<World>();
        m_World->Initialize();

        m_Editor = std::make_unique<Editor::EngineEditor>();
        m_Editor->Initialize();

        m_DummyObjectA = m_World->Spawn<DummyGameObject>();
        m_DummyObjectA->SetName("DummyA");
        m_DummyObjectB = m_World->Spawn<DummyGameObject>(glm::vec3{10.f, 1.f, 2.f});
        m_DummyObjectB->SetName("DummyB");

        glm::vec3 cameraInitialPosition(-12.f, 11.f, 12.f);
        glm::vec3 cameraInitialRotation(-25.f, -47.f, 0.f);
        std::shared_ptr<FlyingCamera> camera = m_World->Spawn<FlyingCamera>(cameraInitialPosition, cameraInitialRotation);
        camera->DisablePilotMode();
        camera->SetName("Camera");

        SpawnCrates();

        auto cube = m_World->Spawn<Cube>(glm::vec3(0.f, 5.f, 0.f));
        cube->SetName("CubePigeon");
        auto anotherMaterial = ResourceManager::CreateMaterial("AnotherMaterial");
        anotherMaterial->SetColor("u_Color", glm::vec4(0.f));

        TextureSettings pigeonTextureSettings{false};
        pigeonTextureSettings.GenerateMipmap = true;
        anotherMaterial->SetTexture("u_Diffuse", ResourceManager::LoadTexture("res/textures/FancyPigeon.png", "Pigeon", pigeonTextureSettings, true), 1);
        cube->SetMaterial(anotherMaterial);

        SpawnBridge();
        SpawnWarrior();
        
        SpawnTransparentObjects();

        auto sphere = m_World->Spawn<Sphere>(glm::vec3(-4.f, 1.f, 2.f));
        auto sphereMaterial = ResourceManager::CreateMaterial("M_Sphere");
        sphereMaterial->SetFloat("u_ReflectionValue", 1.f);
        sphereMaterial->SetColor("u_Color", glm::vec4{0.1f});
        sphere->SetName("Sphere");
        sphere->SetMaterial(sphereMaterial);

        SpawnFloor();
        SpawnLights(*camera);

        auto postProcessing = m_World->Spawn<PostProcessing>();
        postProcessing->SetName("PostProcessing");

        auto skybox = m_World->Spawn<Skybox>();
        skybox->SetName("Skybox");
        skybox->SetDefaultSky();
    }

    void TestCoreSandbox::SpawnTransparentObjects()
    {
        auto flowerMaterial = ResourceManager::CreateMaterial("M_Flower");
        flowerMaterial->SetColor("u_Color", glm::vec4(0.f));
        flowerMaterial->SetTexture("u_Diffuse", ResourceManager::LoadTexture("res/textures/Flower.png", "T_Flower", TextureSettings{true}, true), 1);
        flowerMaterial->SetRenderingMode(MaterialRenderingMode::AlphaCutout);
        
        auto flowerQuad = m_World->Spawn<Quad>(glm::vec3(-2.f, 0.f, -2.f), glm::vec3{0.f, -90.f, 0.f});
        flowerQuad->SetName("Flower");
        flowerQuad->SetMaterial(flowerMaterial);

        auto windowMaterial = ResourceManager::CreateMaterial("M_Window");
        windowMaterial->SetColor("u_Color", glm::vec4(0.f));
        windowMaterial->SetTexture("u_Diffuse", ResourceManager::LoadTexture("res/textures/TransparentWindow.png", "T_Window", TextureSettings{true}, true), 1);
        windowMaterial->SetRenderingMode(MaterialRenderingMode::Transparent);

        glm::vec3 windowRotation{0.f, -90.f, 0.f};

        for(int i = 0; i < 5; i++)
        {
            glm::vec3 windowPosition(static_cast<float>(i) * 2.f, 0.f, -2.f);

            auto windowQuad = m_World->Spawn<Quad>(windowPosition, windowRotation);
            windowQuad->SetName("Window" + std::to_string(i));
            windowQuad->SetMaterial(windowMaterial);
        }
    }

    void TestCoreSandbox::SpawnLights(GameObject& camera)
    {
        int lightIndex = 0;
        for(int x = 0; x < 1; x++)
        {
            for(int z = 0; z < 2; z++)
            {
                glm::vec3 position{static_cast<float>(x) * 2.f, 3.f, static_cast<float>(z) * 2.f};
                auto light = m_World->Spawn<PointLight>(position);
                light->SetName("PointLight" + std::to_string(lightIndex));
                light->SetColor(glm::vec3(1.f, 1.f, 1.f));

                lightIndex++;
            }
        }

        std::shared_ptr<DirectionalLight> mainDirectionalLight = m_World->Spawn<DirectionalLight>();
        mainDirectionalLight->SetPosition(glm::vec3(-6.f, 15.f, 4.f));
        mainDirectionalLight->SetName("MainDirectionalLight");
        mainDirectionalLight->SetRotation(glm::vec3(-45.f, -55.f, 0.f));
        mainDirectionalLight->SetColor(glm::vec3(1.f, 0.82f, 0.635f));
        mainDirectionalLight->SetIntensity(0.3f);

        std::shared_ptr<DirectionalLight> secondaryDirectionalLight = m_World->Spawn<DirectionalLight>();
        secondaryDirectionalLight->SetPosition(glm::vec3(-6.f, 15.f, 4.f));
        secondaryDirectionalLight->SetName("SecondaryDirectionalLight");
        secondaryDirectionalLight->SetRotation(glm::vec3(-45.f, -55.f, 0.f));
        secondaryDirectionalLight->SetColor(glm::vec3(0.82f, 0.875f, 1.f));
        secondaryDirectionalLight->SetIntensity(0.3f);
        secondaryDirectionalLight->SetCastShadowEnabled(false);

        std::shared_ptr<PointLight> pointLight = m_World->Spawn<PointLight>(glm::vec3(-1.f, 0.4f, 10.f));
        pointLight->SetName("PointLight" + std::to_string(lightIndex++));
        pointLight->SetColor(glm::vec3(1.f, 0.f, 0.f));
        pointLight->SetRange(60.f);

        std::shared_ptr<SpotLight> spotLight = m_World->Spawn<SpotLight>(glm::vec3{2.2f, 4.5f, 6.5f}, glm::vec3{-41.f, 37.f, 0.f});
        spotLight->SetName("Spotlight");
        spotLight->SetRange(59.f);
        spotLight->SetInnerCutoffDegrees(22.f);
        spotLight->SetOuterCutoffDegrees(36.5f);

        // auto spotLightComponent = camera.AddComponent<SpotLightComponent>();
        // spotLightComponent->SetRange(100.f);
    }

    void TestCoreSandbox::SpawnCrates()
    {
        std::shared_ptr<Material> crateMaterial = ResourceManager::CreateMaterial("M_Crate");
        crateMaterial->SetColor("u_Color", glm::vec4(0.f)); // When using a texture, we need to set default color to black
        crateMaterial->SetTexture("u_Diffuse", ResourceManager::LoadTexture("res/textures/Container_Diff.png", "Container_Diffuse", TextureSettings{false}, true), 1);
        crateMaterial->SetTexture("u_Specular", ResourceManager::LoadTexture("res/textures/Container_Spec.png", "Container_Specular", TextureSettings{false, false}, true), 2);
        
        int crateIndex = 0;
        for(int x = 0; x < 3; x++)
        {
            for(int z = 0; z < 3; z++)
            {
                glm::vec3 position{static_cast<float>(x) * 2.f, 0.f, static_cast<float>(z) * 2.f};
                auto crate = m_World->Spawn<Cube>(position);
                crate->SetName("Crate" + std::to_string(crateIndex));
                crate->SetMaterial(crateMaterial);

                crateIndex++;
            }
        }
    }

    void TestCoreSandbox::SpawnFloor()
    {
        glm::vec3 floorPosition{4.f, -0.75f, 4.f};
        glm::vec3 floorRotation{0.f};
        glm::vec3 floorScale{44.f, 0.5f, 44.f};
        std::shared_ptr<Cube> floor = m_World->Spawn<Cube>(floorPosition, floorRotation, floorScale);
        floor->SetName("Floor");
    }

    void TestCoreSandbox::SpawnBridge()
    {
        auto bridgeMaterial = ResourceManager::CreateMaterial("M_Bridge");
        bridgeMaterial->SetColor("u_Color", glm::vec4(0.f));
        bridgeMaterial->SetTexture("u_Diffuse", ResourceManager::LoadTexture("res/textures/Atlas04_Diff.png", "T_Bridge_Diffuse", TextureSettings{false}), 1);

        auto bridgeModel = ResourceManager::LoadModel("res/models/Bridge.fbx", "Bridge");

        auto bridge = m_World->Spawn<Model>(glm::vec3(0.f, 0.f, -20.f));
        bridge->Setup(bridgeModel, bridgeMaterial);
        bridge->SetName("Bridge");
    }

    void TestCoreSandbox::SpawnWarrior()
    {
        auto warriorMaterial = ResourceManager::CreateMaterial("M_Liz");
        warriorMaterial->SetColor("u_Color", glm::vec4(0.f));
        warriorMaterial->SetTexture("u_Diffuse", ResourceManager::LoadTexture("res/textures/liz/T_Liz_Diffuse.png", "T_Liz_Diffuse", TextureSettings{false}), 0);
        warriorMaterial->SetTexture("u_Specular", ResourceManager::LoadTexture("res/textures/liz/T_Liz_Specular.png", "T_Liz_Specular", TextureSettings{false}), 1);
        
        auto warriorModel = ResourceManager::LoadModel("res/models/PigeonsAttack_Liz.fbx", "Liz");

        glm::vec3 spawnPosition{0.f, -0.5f, -4.f};
        glm::vec3 spawnRotation{0.f, -31.f, 0.f};
        glm::vec3 spawnScale{0.25f};

        auto warrior = m_World->Spawn<Model>(spawnPosition, spawnRotation, spawnScale);
        warrior->Setup(warriorModel, warriorMaterial);
        warrior->SetName("Warrior");
    }

    TestCoreSandbox::~TestCoreSandbox()
    {
        m_World->Shutdown();
        m_World.reset();

        ResourceManager::UnloadAll();
    }

    void TestCoreSandbox::Setup(GLFWwindow* Window)
    {
        Test::Setup(Window);

        m_World->Setup();
    }

    void TestCoreSandbox::OnUpdate(float deltaTime)
    {
        m_World->Update(deltaTime);
        m_Editor->Update(deltaTime, *m_World);
    }

    void TestCoreSandbox::OnRender()
    {
        m_World->Render();
    }

    void TestCoreSandbox::OnImGuiRender()
    {
        m_Editor->RenderGUI(*m_World);
    }
}
