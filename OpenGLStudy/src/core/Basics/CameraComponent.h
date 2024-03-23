#pragma once

#include "core/GameObject/Component.h"
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

class CameraComponent : public Component 
{
    GENERATE_COMPONENT_BODY(CameraComponent)

public:

    void Initialize() override;
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;

private:

    float m_Fov{45.f};
    float m_NearPlane{0.1f};
    float m_FarPlane{100.f};
    glm::vec3 m_Up{0.f, 1.f, 0.f};
};
