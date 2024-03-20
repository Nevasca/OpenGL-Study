#include "GameObject.h"

void GameObject::Initialize()
{ }

void GameObject::Start()
{ }

void GameObject::Update(float deltaTime)
{ }

void GameObject::Destroy()
{
    b_IsPendingDestroy = true;
}

void GameObject::SetPosition(const glm::vec3& position)
{
    m_Transform.SetPosition(position);
}

void GameObject::SetRotation(const glm::vec3& eulerRotation)
{
    m_Transform.SetRotation(eulerRotation);
}

void GameObject::SetScale(const glm::vec3& scale)
{
    m_Transform.SetScale(scale);
}
