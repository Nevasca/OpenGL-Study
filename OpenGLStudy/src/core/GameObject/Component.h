#pragma once

#define GENERATE_COMPONENT_BODY(ClassName) ClassName(GameObject& owner)\
    : Component(owner) { }

class GameObject;

class Component
{
public:

    Component(GameObject& owner);
    virtual ~Component() = default;
    
    virtual void Initialize();
    virtual void Start();
    virtual void Update(float deltaTime);
    virtual void Destroy();
    
    GameObject& GetOwner() const { return m_Owner; }

private:

    GameObject& m_Owner;
};
