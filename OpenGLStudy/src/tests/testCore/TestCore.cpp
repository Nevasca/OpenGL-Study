#include "TestCore.h"

#include <memory>

#include "DummyComponent.h"
#include "LegacyShader.h"
#include "core/ResourceManager.h"
#include "core/Basics/Objects/FlyingCamera.h"
#include "core/Basics/Objects/Cube.h"
#include "core/Basics/Objects/DirectionalLight.h"
#include "core/Basics/Components//DirectionalLightComponent.h"

namespace tests
{
    TestCore::TestCore()
    {
        ResourceManager::LoadDefaultResources();

        m_World = std::make_unique<World>();
        m_World->Initialize();

        m_DummyObjectA = m_World->Spawn<DummyGameObject>();
        m_DummyObjectB = m_World->Spawn<DummyGameObject>(glm::vec3{10.f, 1.f, 2.f});

        glm::vec3 cameraInitialPosition(0.f, 0.f, 5.f);
        glm::vec3 cameraInitialRotation(0.f, 180.f, 0.f);
        std::shared_ptr<FlyingCamera> camera = m_World->Spawn<FlyingCamera>(cameraInitialPosition, cameraInitialRotation);

        for(int x = 0; x < 100; x++)
        {
            for(int z = 0; z < 100; z++)
            {
                glm::vec3 position{static_cast<float>(x) * 2.f, 0.f, static_cast<float>(z) * 2.f};
                m_World->Spawn<Cube>(position);
            }
        }

        std::shared_ptr<DirectionalLight> mainDirectionalLight = m_World->Spawn<DirectionalLight>();
        mainDirectionalLight->SetRotation(glm::vec3(45.f, 0.f, 0.f));
        mainDirectionalLight->SetColor(glm::vec3(1.f, 0.82f, 0.635f));
        mainDirectionalLight->SetIntensity(1.5f);

        std::shared_ptr<DirectionalLight> secondaryDirectionalLight = m_World->Spawn<DirectionalLight>();
        secondaryDirectionalLight->SetRotation(glm::vec3(-45.f, 45.f, 0.f));
        std::shared_ptr<DirectionalLightComponent> testGetDirectionalLight = secondaryDirectionalLight->GetComponent<DirectionalLightComponent>();
        testGetDirectionalLight->SetColor(glm::vec3(0.82f, 0.875f, 1.f));
        testGetDirectionalLight->SetIntensity(0.5f);
        
        // cube->AddComponent<DummyComponent>();
    }

    TestCore::~TestCore()
    {
        m_World->Shutdown();
        ResourceManager::UnloadAll();
    }

    void TestCore::OnUpdate(float DeltaTime)
    {
        m_World->Update(DeltaTime);
    }

    void TestCore::OnRender()
    {
        m_World->Render();
    }

    void TestCore::OnImGuiRender()
    {
        
    }
}
