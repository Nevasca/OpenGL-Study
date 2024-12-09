#pragma once
#include "core/GameObject/GameObject.h"

class Material;
class MeshComponent;

class Sphere : public GameObject
{
    GENERATE_OBJECT_BODY(Sphere)

public:

    void Initialize() override;
    void Destroy() override;

    void SetMaterial(const std::shared_ptr<Material>& material) const;

private:

    std::shared_ptr<MeshComponent> m_MeshComponent{};
};
