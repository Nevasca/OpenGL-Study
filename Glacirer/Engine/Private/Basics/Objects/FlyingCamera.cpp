#include "Basics/Objects/FlyingCamera.h"

#include "Basics/Components/CameraComponent.h"
#include "Basics/Components/PilotComponent.h"
#include "Basics/Components/PilotCameraController.h"

namespace Glacirer
{
    void FlyingCamera::Initialize()
    {
        GameObject::Initialize();

        std::shared_ptr<CameraComponent> cameraComponent = AddComponent<CameraComponent>();
        std::shared_ptr<PilotComponent> pilotComponent = AddComponent<PilotComponent>();

        m_CameraController = AddComponent<PilotCameraController>();
        m_CameraController->Setup(cameraComponent, pilotComponent);
    }

    void FlyingCamera::EnablePilotMode()
    {
        m_CameraController->EnablePilotMode();
    }

    void FlyingCamera::DisablePilotMode()
    {
        m_CameraController->DisablePilotMode();
    }
}
