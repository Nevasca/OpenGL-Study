#pragma once
#include <iostream>

#include "Transform.h"

class GameObject
{
public:

    GameObject() = default;
    virtual ~GameObject() = default;

    virtual void Initialize();
    virtual void Start();
    virtual void Update(float deltaTime);
    virtual void Destroy();
    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::vec3& eulerRotation);
    void SetScale(const glm::vec3& scale);

    inline glm::vec3 GetPosition() const { return m_Transform.GetPosition(); }
    inline glm::vec3 GetRotation() const { return m_Transform.GetRotation(); }
    inline glm::vec3 GetScale() const { return m_Transform.GetScale(); }
    inline Transform& GetTransform() { return m_Transform; }
    inline bool IsPendingDestroy() const { return b_IsPendingDestroy; }

protected:

    Transform m_Transform{};

private:

    bool b_IsPendingDestroy{false};
};
