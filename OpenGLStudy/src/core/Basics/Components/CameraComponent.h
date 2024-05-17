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
    glm::mat4 GetViewNoTranslationMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    void SetFov(const float fov);
    float GetFov() const { return m_Fov; }
    void SetNearPlane(const float nearPlane) { m_NearPlane = nearPlane; }
    float GetNearPlane() const { return m_NearPlane; }
    void SetFarPlane(const float farPlane) { m_FarPlane = farPlane; }
    float GetFarPlane() const { return m_FarPlane; }

private:

    float m_Fov{45.f};
    float m_NearPlane{0.1f};
    float m_FarPlane{1000.f};
    glm::vec3 m_Up{0.f, 1.f, 0.f};
};
