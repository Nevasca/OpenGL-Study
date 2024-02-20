#include "Camera.h"

#include "glm/detail/type_quat.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

Camera::Camera(float ScreenWidth, float ScreenHeight)
    : m_ScreenWidth(ScreenWidth), m_ScreenHeight(ScreenHeight)
{
    m_Projection = glm::perspective(glm::radians(m_Fov), m_ScreenWidth / m_ScreenHeight, m_NearPlane, m_FarPlane);
}

glm::vec3 Camera::GetForwardVector() const
{
    return m_Forward;
}

glm::vec3 Camera::GetUpVector() const
{
    return m_Up;
}

glm::vec3 Camera::GetRightVector() const
{
    return glm::normalize(glm::cross(m_Forward, m_Up));
}

glm::vec3 Camera::GetRotation() const
{
    return m_Rotation;
}

void Camera::SetRotation(const glm::vec3& EulerRotation)
{
    m_Rotation = EulerRotation;
    
    if(m_ClampPitchEnabled)
    {
        m_Rotation.x = glm::clamp(m_Rotation.x, m_MinPitch, m_MaxPitch);  
    }
    
    if(m_ClampYawEnabled)
    {
        m_Rotation.y = glm::clamp(m_Rotation.y, m_MinYaw, m_MaxYaw);
    }
    
    // TODO: Support roll
    
    // Calculate Euler angles for the new camera forward based on pitch and yaw (we are not interested in roll for that)
    // For better math explanation, check https://learnopengl.com/Getting-started/Camera
    glm::vec3 cameraDirection{};
    cameraDirection.x = cos(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x));
    cameraDirection.y = sin(glm::radians(m_Rotation.x));
    cameraDirection.z = sin(glm::radians(m_Rotation.y)) * cos(glm::radians(m_Rotation.x));
    
    m_Forward = glm::normalize(cameraDirection);
    
    // This approach takes de roll into consideration as well, but we need to invert x and y movement
    // glm::quat rotationQuat = glm::quat(glm::radians(m_Rotation));
    // glm::mat3 rotationMatrix = glm::mat3_cast(rotationQuat);
    // glm::vec3 forward{0.f, 0.f, 1.f};
    //
    // m_Forward = glm::normalize(rotationMatrix * forward);
}

float Camera::GetFov() const
{
    return m_Fov;
}

void Camera::SetFov(float Fov)
{
    m_Fov = glm::clamp(Fov, m_MinFov, m_MaxFov);

    m_Projection = glm::perspective(glm::radians(m_Fov), m_ScreenWidth / m_ScreenHeight, m_NearPlane, m_FarPlane);
}

glm::mat4 Camera::GetViewMatrix() const
{
    // Since the camera front is something like (0.f, 0.f, -1.f), adding it to the position will make the target position be in front of the camera
    // Ensuring that however we move, the camera keeps looking at the target direction (0.f, 0.f, -1.f)
    return glm::lookAt(Position, Position + m_Forward, m_Up);
}

glm::mat4 Camera::GetViewProjectionMatrix() const
{
    return m_Projection * GetViewMatrix();
}
