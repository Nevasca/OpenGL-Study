#pragma once
#include "glm/fwd.hpp"
#include "glm/vec3.hpp"

class Transform
{
public:

    Transform() = default;
    Transform(const glm::vec3& position, const glm::vec3& eulerRotation, const glm::vec3& scale);

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::vec3& eulerRotation);
    void SetScale(const glm::vec3& scale);
    inline glm::vec3 GetPosition() const { return m_Position; }
    inline glm::vec3 GetRotation() const { return m_Rotation; }
    inline glm::vec3 GetScale() const { return m_Scale; }
    glm::vec3 GetForwardVector() const;
    glm::mat4 GetMatrix() const;

private:

    glm::vec3 m_Position{0.f};
    glm::vec3 m_Rotation{0.f};
    glm::vec3 m_Scale{1.f};
};