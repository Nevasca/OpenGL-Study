#pragma once
#include "GameObject/Component.h"

namespace Glacirer
{
    class PilotComponent;
    class CameraComponent;

    class ENGINE_API PilotCameraController : INHERIT_FROM_COMPONENT(PilotCameraController)
    {
        GENERATE_COMPONENT_BODY(PilotCameraController)
    
    public:

        void Initialize() override;
        void Destroy() override;
        void Update(float deltaTime) override;
        void Enable() override;
        void Disable() override;

        void Setup(const std::weak_ptr<CameraComponent>& camera, const std::weak_ptr<PilotComponent>& pilot);
        void UpdateCameraZoom(double YScrollOffset);
        void EnablePilotMode();
        void DisablePilotMode();

    private:

        std::weak_ptr<CameraComponent> m_Camera;
        std::weak_ptr<PilotComponent> m_Pilot;
        float m_MinFov{1.f};
        float m_MaxFov{90.f};
        bool bIsPilotEnabled{true};
    };
}
