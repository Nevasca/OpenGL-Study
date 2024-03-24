#include "TestCore.h"

#include <memory>

#include "DummyComponent.h"
#include "Shader.h"
#include "core/Basics/Objects/FlyingCamera.h"
#include "core/Basics/Components/MeshComponent.h"
#include "core/Rendering/Primitive.h"

namespace tests
{
    TestCore::TestCore()
    {
        m_World = std::make_unique<World>();
        m_World->Initialize();
        m_DummyObjectA = m_World->Spawn<DummyGameObject>();
        m_DummyObjectB = m_World->Spawn<DummyGameObject>(glm::vec3{10.f, 1.f, 2.f});

        glm::vec3 cameraInitialPosition(0.f, 0.f, 5.f);
        glm::vec3 cameraInitialRotation(0.f, 180.f, 0.f);
        std::shared_ptr<FlyingCamera> camera = m_World->Spawn<FlyingCamera>(cameraInitialPosition, cameraInitialRotation);

        std::shared_ptr<GameObject> cube = m_World->Spawn<GameObject>();
        std::shared_ptr<MeshComponent> meshComponent = cube->AddComponent<MeshComponent>();
        meshComponent->SetMesh(Primitive::CreateCube());
        meshComponent->SetShader(std::make_shared<Shader>("res/core/shaders/Default.glsl"));
        // cube->AddComponent<DummyComponent>();
    }

    TestCore::~TestCore()
    {
        m_World->Shutdown();
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
