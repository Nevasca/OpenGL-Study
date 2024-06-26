#pragma once
#include "core/GameObject/GameObject.h"

class PilotCameraController;

class FlyingCamera : public GameObject
{
    GENERATE_OBJECT_BODY(FlyingCamera)
    
public:

    void Initialize() override;

    void EnablePilotMode();
    void DisablePilotMode();

private:

    std::shared_ptr<PilotCameraController> m_CameraController{}; 
};
