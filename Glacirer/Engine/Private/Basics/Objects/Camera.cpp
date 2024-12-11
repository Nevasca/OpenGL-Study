#include "Basics/Objects/Camera.h"
#include "Basics/Components/CameraComponent.h"

namespace Glacirer
{
    void Camera::Initialize()
    {
        GameObject::Initialize();

        m_CameraComponent = AddComponent<CameraComponent>();
    }
}
