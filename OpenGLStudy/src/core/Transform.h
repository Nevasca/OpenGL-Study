#pragma once
#include "glm/fwd.hpp"
#include "glm/vec3.hpp"

class Transform
{
public:

    void SetPosition(glm::vec3 position);
    void SetRotation(glm::vec3 eulerRotation);
    void SetScale(glm::vec3 scale);
    glm::vec3 GetPosition() const;
    glm::vec3 GetRotation() const;
    glm::vec3 GetScale() const;
    glm::vec3 GetForwardVector() const;
    glm::mat4 GetMatrix() const;

private:

    glm::vec3 m_Position{0.f};
    glm::vec3 m_Rotation{0.f};
    glm::vec3 m_Scale{1.f};
};
