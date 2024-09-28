#include "TestCore.h"

#include "core/ResourceManager.h"
#include "core/Basics/Objects/Cube.h"
#include "core/Basics/Objects/DirectionalLight.h"
#include "core/Basics/Objects/FlyingCamera.h"
#include "core/Basics/Objects/PostProcessing.h"
#include "core/Basics/Objects/Sphere.h"

namespace tests
{
    TestCore::TestCore()
    {
        ResourceManager::LoadDefaultResources();

        m_World = std::make_unique<World>();
        m_World->Initialize();

        m_Editor = std::make_unique<Editor::EngineEditor>();
        m_Editor->Initialize();

        SpawnDefaultWorld();
    }

    TestCore::~TestCore()
    {
        m_World->Shutdown();
        m_World.reset();

        ResourceManager::UnloadAll();
    }

    void TestCore::OnUpdate(float deltaTime)
    {
        Test::OnUpdate(deltaTime);

        m_World->Update(deltaTime);
        m_Editor->Update(deltaTime, *m_World);
    }

    void TestCore::OnRender()
    {
        Test::OnRender();

        m_World->Render();
    }

    void TestCore::OnImGuiRender()
    {
        Test::OnImGuiRender();

        m_Editor->RenderGUI(*m_World);
    }

    void TestCore::SpawnDefaultWorld()
    {
        glm::vec3 cameraStartPosition{-3.f, 3.42f, -12.f};
        glm::vec3 cameraStartRotation{-14.4f, -166.2f, 0.f};
        std::shared_ptr<FlyingCamera> flyingCamera = m_World->Spawn<FlyingCamera>(cameraStartPosition, cameraStartRotation);
        flyingCamera->SetName("Camera");
        flyingCamera->DisablePilotMode();

        glm::vec3 directionalLightPosition{0.f, 3.f, 0.f};
        glm::vec3 directionalLightRotation{0.f, 120.f, 80.f};
        std::shared_ptr<DirectionalLight> directionalLight = m_World->Spawn<DirectionalLight>(directionalLightPosition, directionalLightRotation);
        directionalLight->SetName("DirectionalLight");
        directionalLight->SetColor(glm::vec3{1.f, 0.89f, 0.83f});
        directionalLight->SetIntensity(1.2f);

        std::shared_ptr<PostProcessing> postProcessing = m_World->Spawn<PostProcessing>();
        postProcessing->SetName("PostProcessing");

        std::shared_ptr<Cube> cube = m_World->Spawn<Cube>(glm::vec3{0.f, 0.5f, 0.f});
        cube->SetName("Cube");

        glm::vec3 floorPosition{0.f, -0.125f, 0.f};
        glm::vec3 floorRotation{0.f};
        glm::vec3 floorScale{10.f, 0.25f, 10.f};
        std::shared_ptr<Cube> floor = m_World->Spawn<Cube>(floorPosition, floorRotation, floorScale);
        floor->SetName("Floor");
    }
}
