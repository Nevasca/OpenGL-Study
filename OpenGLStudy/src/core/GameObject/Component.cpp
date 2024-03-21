#include "Component.h"

Component::Component(GameObject& owner)
    : m_Owner(owner)
{ }

void Component::Initialize()
{ }

void Component::Start()
{ }

void Component::Update(float deltaTime)
{ }

void Component::Destroy()
{ }
