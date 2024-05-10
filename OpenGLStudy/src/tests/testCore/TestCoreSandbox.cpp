#include "TestCoreSandbox.h"

#include <memory>

#include "legacy/LegacyShader.h"
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
#include "core/Basics/Objects/Sphere.h"
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
        glm::vec3 cameraInitialRotation(29.f, 133.f, 0.f);
        std::shared_ptr<FlyingCamera> camera = m_World->Spawn<FlyingCamera>(cameraInitialPosition, cameraInitialRotation);
        camera->SetName("Camera");

        int cubeIndex = 0;
        for(int x = 0; x < 5; x++)
        {
            for(int z = 0; z < 5; z++)
            {
                glm::vec3 position{static_cast<float>(x) * 2.f, 0.f, static_cast<float>(z) * 2.f};
                auto cube = m_World->Spawn<Cube>(position);
                cube->SetName("Cube" + std::to_string(cubeIndex));

                cubeIndex++;
            }
        }

        std::shared_ptr<Material> defaultMaterial = ResourceManager::GetDefaultMaterial();
        defaultMaterial->SetColor("u_Color", glm::vec4(0.f)); // When using a texture, we need to set default color to black
        defaultMaterial->SetTexture("u_Diffuse", ResourceManager::LoadTexture("res/textures/Container_Diff.png", "Container_Diffuse", false, true), 0);
        defaultMaterial->SetTexture("u_Specular", ResourceManager::LoadTexture("res/textures/Container_Spec.png", "Container_Specular", false, true), 1);

        auto cube = m_World->Spawn<Cube>(glm::vec3(0.f, 5.f, 0.f));
        cube->SetName("CubePigeon");
        auto anotherMaterial = ResourceManager::CreateMaterial("AnotherMaterial");
        anotherMaterial->SetColor("u_Color", glm::vec4(0.f));
        anotherMaterial->SetTexture("u_Diffuse", ResourceManager::LoadTexture("res/textures/FancyPigeon.png", "Pigeon", false, true), 0);
        cube->SetMaterial(anotherMaterial);

        auto bridgeMaterial = ResourceManager::CreateMaterial("M_Bridge");
        bridgeMaterial->SetColor("u_Color", glm::vec4(0.f));
        bridgeMaterial->SetTexture("u_Diffuse", ResourceManager::LoadTexture("res/textures/Atlas04_Diff.png", "T_Bridge_Diffuse", false), 0);
        auto bridgeModel = ResourceManager::LoadModel("res/models/Bridge.fbx", "Bridge");

        auto bridge = m_World->Spawn<Model>(glm::vec3(0.f, 0.f, -20.f));
        bridge->Setup(bridgeModel, bridgeMaterial);
        bridge->SetName("Bridge");

        SpawnTransparentObjects();

        auto sphere = m_World->Spawn<Sphere>(glm::vec3(-4.f, 0.f, 0.f));
        sphere->SetName("Sphere");

        SpawnLights(*camera);

        auto postProcessing = m_World->Spawn<PostProcessing>();
        postProcessing->SetName("PostProcessing");
    }

    void TestCoreSandbox::SpawnTransparentObjects()
    {
        auto flowerMaterial = ResourceManager::CreateMaterial("M_Flower");
        flowerMaterial->SetColor("u_Color", glm::vec4(0.f));
        flowerMaterial->SetTexture("u_Diffuse", ResourceManager::LoadTexture("res/textures/Flower.png", "T_Flower", true, true), 0);
        
        auto flowerQuad = m_World->Spawn<Quad>(glm::vec3(-2.f, 0.f, -2.f), glm::vec3{0.f, -90.f, 0.f});
        flowerQuad->SetName("Flower");
        flowerQuad->SetMaterial(flowerMaterial);

        auto windowMaterial = ResourceManager::CreateMaterial("M_Window");
        windowMaterial->SetColor("u_Color", glm::vec4(0.f));
        windowMaterial->SetTexture("u_Diffuse", ResourceManager::LoadTexture("res/textures/TransparentWindow.png", "T_Window", true, true), 0);
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
        for(int x = 0; x < 4; x++)
        {
            for(int z = 0; z < 4; z++)
            {
                glm::vec3 position{static_cast<float>(x) * 50.f, 3.f, static_cast<float>(z) * 50.f};
                auto light = m_World->Spawn<PointLight>(position);
                light->SetName("PointLight" + std::to_string(lightIndex));
                light->SetColor(glm::vec3(1.f, 0.f, 1.f));

                lightIndex++;
            }
        }

        std::shared_ptr<DirectionalLight> mainDirectionalLight = m_World->Spawn<DirectionalLight>();
        mainDirectionalLight->SetName("MainDirectionalLight");
        mainDirectionalLight->SetRotation(glm::vec3(45.f, 0.f, 0.f));
        mainDirectionalLight->SetColor(glm::vec3(1.f, 0.82f, 0.635f));
        mainDirectionalLight->SetIntensity(0.3f);

        std::shared_ptr<DirectionalLight> secondaryDirectionalLight = m_World->Spawn<DirectionalLight>();
        secondaryDirectionalLight->SetName("SecondaryDirectionalLight");
        secondaryDirectionalLight->SetRotation(glm::vec3(-45.f, 45.f, 0.f));
        std::shared_ptr<DirectionalLightComponent> testGetDirectionalLight = secondaryDirectionalLight->GetComponent<DirectionalLightComponent>();
        testGetDirectionalLight->SetColor(glm::vec3(0.82f, 0.875f, 1.f));
        testGetDirectionalLight->SetIntensity(0.2f);

        std::shared_ptr<PointLight> pointLight = m_World->Spawn<PointLight>(glm::vec3(0.f, 3.f, 0.f));
        pointLight->SetName("PointLight" + std::to_string(lightIndex++));
        pointLight->SetColor(glm::vec3(1.f, 0.f, 0.f));
        pointLight->SetRange(60.f);

        auto spotLightComponent = camera.AddComponent<SpotLightComponent>();
        spotLightComponent->SetRange(100.f);
    }

    TestCoreSandbox::~TestCoreSandbox()
    {
        m_World->Shutdown();
        m_World.reset();

        ResourceManager::UnloadAll();
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
