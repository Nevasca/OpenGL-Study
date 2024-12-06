#pragma once
#include "GameObject/GameObject.h"

class Material;
class MeshComponent;

class ENGINE_API Sphere : public GameObject
{
    GENERATE_OBJECT_BODY(Sphere)

public:

    void Initialize() override;
    void Destroy() override;

    void SetMaterial(const std::shared_ptr<Material>& material) const;

private:

    std::shared_ptr<MeshComponent> m_MeshComponent{};
};
