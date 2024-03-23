#include "FlyCamera.h"

#include "core/Basics/Components/CameraComponent.h"
#include "core/Basics/Components/PilotComponent.h"
#include "core/Basics/Components/PilotCameraController.h"

class CameraComponent;

void FlyCamera::Initialize()
{
    GameObject::Initialize();

    std::shared_ptr<CameraComponent> cameraComponent = AddComponent<CameraComponent>();
    std::shared_ptr<PilotComponent> pilotComponent = AddComponent<PilotComponent>();
    std::shared_ptr<PilotCameraController> cameraController = AddComponent<PilotCameraController>();
    
    cameraController->Setup(cameraComponent, pilotComponent);
}
