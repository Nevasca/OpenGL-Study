#pragma once

//Children component classes need to have <memory>, <string> and <functional> included
//so they can use the generate macro
#include <memory>
#include <string>
#include <functional>

#include <glm/vec3.hpp>

#define GENERATE_COMPONENT_BODY(ClassName) friend class GameObject;\
    public:\
    ClassName(GameObject& owner)\
    : Component(owner) { }\
    std::string GetName() const override { return #ClassName; }\
    static int GetClassHash()\
    {\
        std::hash<std::string> hasher;\
        return static_cast<int>(hasher(#ClassName));\
    }\
    int GetHash() override { return GetClassHash(); }\
    private:\
    std::shared_ptr<ClassName> m_This{};\
    void SetThis(const std::shared_ptr<ClassName>& InThis) { m_This = InThis; }\
    std::shared_ptr<ClassName> GetThis() const { return m_This; }


class World;
class Transform;
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
    virtual void Enable();
    virtual void Disable();
    void SetOwnerPosition(const glm::vec3& position) const;
    void SetOwnerRotation(const glm::vec3& eulerRotation) const;
    void SetOwnerScale(const glm::vec3& scale) const;
    glm::vec3 GetOwnerPosition() const;
    glm::vec3 GetOwnerRotation() const;
    glm::vec3 GetOwnerScale() const;
    Transform& GetOwnerTransform() const;
    World& GetWorld() const;
    
    GameObject& GetOwner() const { return m_Owner; }
    bool IsEnabled() const { return bIsEnabled; }
    unsigned int GetId() const { return m_Id; }
    virtual std::string GetName() const { return "Component"; }

    static int GetClassHash() { return 0; }
    virtual int GetHash() { return GetClassHash(); } 

protected:

    Component(GameObject& owner);

    bool bIsEnabled{true};

private:

    GameObject& m_Owner;
    unsigned int m_Id{0};
};
