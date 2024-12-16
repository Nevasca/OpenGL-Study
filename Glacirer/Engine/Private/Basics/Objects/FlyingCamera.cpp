#include "Basics/Objects/FlyingCamera.h"

#include "Basics/Components/CameraComponent.h"
#include "Basics/Components/PilotComponent.h"
#include "Basics/Components/PilotCameraController.h"

namespace Glacirer
{
    void FlyingCamera::Initialize()
    {
        GameObject::Initialize();

        std::weak_ptr<CameraComponent> cameraComponent = AddComponent<CameraComponent>();
        std::weak_ptr<PilotComponent> pilotComponent = AddComponent<PilotComponent>();

        m_CameraController = AddComponent<PilotCameraController>();
        std::shared_ptr<PilotCameraController> cameraController = m_CameraController.lock();
        assert(cameraController);

        cameraController->Setup(cameraComponent, pilotComponent);
    }

    void FlyingCamera::EnablePilotMode()
    {
        std::shared_ptr<PilotCameraController> cameraController = m_CameraController.lock();

        if(cameraController)
        {
            cameraController->EnablePilotMode();
        }
    }

    void FlyingCamera::DisablePilotMode()
    {
        std::shared_ptr<PilotCameraController> cameraController = m_CameraController.lock();

        if(cameraController)
        {
            cameraController->DisablePilotMode();
        }
    }
}
