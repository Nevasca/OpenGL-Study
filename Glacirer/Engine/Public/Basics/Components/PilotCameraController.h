#pragma once
#include "GameObject/Component.h"

class PilotComponent;
class CameraComponent;

class PilotCameraController : public Component
{
    GENERATE_COMPONENT_BODY(PilotCameraController)
    
public:

    void Initialize() override;
    void Destroy() override;
    void Update(float deltaTime) override;
    void Enable() override;
    void Disable() override;

    void Setup(const std::shared_ptr<CameraComponent>& camera, const std::shared_ptr<PilotComponent>& pilot);
    void UpdateCameraZoom(double YScrollOffset);
    void EnablePilotMode();
    void DisablePilotMode();

private:

    std::shared_ptr<CameraComponent> m_Camera;
    std::shared_ptr<PilotComponent> m_Pilot;
    float m_MinFov{1.f};
    float m_MaxFov{90.f};
    bool bIsPilotEnabled{true};
};
