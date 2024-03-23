#pragma once

//Children component classes need to have <memory> included
//so they can use the generate macro that contains shared_ptr
#include <memory>

#include "glm/vec3.hpp"

#define GENERATE_COMPONENT_BODY(ClassName) friend class GameObject;\
    public:\
    ClassName(GameObject& owner)\
    : Component(owner) { }\
    private:\
    std::shared_ptr<ClassName> m_This{};\
    void SetThis(const std::shared_ptr<ClassName>& InThis) { m_This = InThis; }\
    std::shared_ptr<ClassName> GetThis() { return m_This; }

class GameObject;

class Component
{
    friend class GameObject;

public:

    virtual ~Component() = default;
    
    virtual void Initialize();
    virtual void Start();
    virtual void Update(float deltaTime);
    virtual void Destroy();
    void SetOwnerPosition(const glm::vec3& position) const;
    void SetOwnerRotation(const glm::vec3& eulerRotation) const;
    void SetOwnerScale(const glm::vec3& scale) const;
    glm::vec3 GetOwnerPosition() const;
    glm::vec3 GetOwnerRotation() const;
    glm::vec3 GetOwnerScale() const;
    
    GameObject& GetOwner() const { return m_Owner; }

protected:

    Component(GameObject& owner);

private:

    GameObject& m_Owner;
};
