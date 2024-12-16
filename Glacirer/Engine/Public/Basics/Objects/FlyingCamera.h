#pragma once
#include "GameObject/GameObject.h"

namespace Glacirer
{
    class PilotCameraController;

    class ENGINE_API FlyingCamera : public GameObject
    {
        GENERATE_OBJECT_BODY(FlyingCamera)
    
    public:

        void Initialize() override;

        void EnablePilotMode();
        void DisablePilotMode();

    private:

        std::weak_ptr<PilotCameraController> m_CameraController{}; 
    };
}
