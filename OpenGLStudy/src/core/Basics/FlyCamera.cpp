#include "FlyCamera.h"

#include "CameraComponent.h"
#include "PilotComponent.h"
#include "PilotCameraController.h"

class CameraComponent;

void FlyCamera::Initialize()
{
    GameObject::Initialize();

    std::shared_ptr<CameraComponent> cameraComponent = AddComponent<CameraComponent>();
    std::shared_ptr<PilotComponent> pilotComponent = AddComponent<PilotComponent>();
    std::shared_ptr<PilotCameraController> cameraController = AddComponent<PilotCameraController>();
    
    cameraController->Setup(cameraComponent, pilotComponent);
}
