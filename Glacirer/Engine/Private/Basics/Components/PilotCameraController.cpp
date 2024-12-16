#include "Basics/Components/PilotCameraController.h"

#include <GLFW/glfw3.h>

#include "Basics/Components/CameraComponent.h"
#include "Basics/Components/PilotComponent.h"
#include "Application.h"
#include "Input.h"
#include <glm/common.hpp>

namespace
{
    // Using a global var instance of "this" so we can use the instance on required global function callback
    // for glfw scroll callback
    // This is a workaround, not sure what's the best approach on these cases...
    Glacirer::PilotCameraController* g_PilotCameraComponent;
    GLFWscrollfun g_PreviousScrollCallback;

    void ScrollCallbackWrapper(GLFWwindow* Window, double XOffset, double YOffset)
    {
        if(g_PreviousScrollCallback)
        {
            g_PreviousScrollCallback(Window, XOffset, YOffset);
        }

        g_PilotCameraComponent->UpdateCameraZoom(YOffset);
    }
}


namespace Glacirer
{
    void PilotCameraController::Initialize()
    {
        Component::Initialize();

        g_PilotCameraComponent = this;
    }

    void PilotCameraController::Destroy()
    {
        Component::Destroy();

        g_PilotCameraComponent = nullptr;
        g_PreviousScrollCallback = nullptr;

        m_Camera.reset();
        m_Pilot.reset();
    }

    void PilotCameraController::Setup(const std::weak_ptr<CameraComponent>& camera, const std::weak_ptr<PilotComponent>& pilot)
    {
        m_Camera = camera;
        m_Pilot = pilot;
    }

    void PilotCameraController::Update(float deltaTime)
    {
        Component::Update(deltaTime);

        if(bIsPilotEnabled && Input::GetKeyDown(GLFW_KEY_ESCAPE))
        {
            DisablePilotMode();
        }
        else if(!bIsPilotEnabled && Input::GetKeyDown(GLFW_KEY_F))
        {
            EnablePilotMode();
        }
    }

    void PilotCameraController::UpdateCameraZoom(double YScrollOffset)
    {
        if(!bIsPilotEnabled)
        {
            return;
        }

        std::shared_ptr<CameraComponent> camera = m_Camera.lock();

        if(!camera)
        {
            return;
        }
        
        float fov = camera->GetFov();
        fov -= static_cast<float>(YScrollOffset);
        fov = glm::clamp(fov, m_MinFov, m_MaxFov);

        camera->SetFov(fov);
    }

    void PilotCameraController::EnablePilotMode()
    {
        bIsPilotEnabled = true;

        std::shared_ptr<PilotComponent> pilotComponent = m_Pilot.lock();
        if(pilotComponent)
        {
            pilotComponent->Enable();
        }
    }

    void PilotCameraController::DisablePilotMode()
    {
        bIsPilotEnabled = false;

        std::shared_ptr<PilotComponent> pilotComponent = m_Pilot.lock();
        if(pilotComponent)
        {
            pilotComponent->Disable();
        }
    }

    void PilotCameraController::Enable()
    {
        Component::Enable();

        // glfwSetScrollCallback overrides whatever callback is currently set
        // it also returns the previous callback when setting a new one
        // so we store it to also call the previous behavior if any on ScrollCallbackWrapper 
        g_PreviousScrollCallback = glfwSetScrollCallback(Application::GetCurrentWindow(), ScrollCallbackWrapper);   
    }

    void PilotCameraController::Disable()
    {
        Component::Disable();

        glfwSetScrollCallback(Application::GetCurrentWindow(), g_PreviousScrollCallback);
    }
}
