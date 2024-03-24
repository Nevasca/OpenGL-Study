#pragma once
#include "core/GameObject/GameObject.h"


class Shader;
class MeshComponent;

class Cube : public GameObject
{
    GENERATE_OBJECT_BODY(Cube)
    
public:
    
    void Initialize() override;
    void Destroy() override;
    
    void SetShader(const std::shared_ptr<Shader>& shader) const;

private:

    std::shared_ptr<MeshComponent> m_MeshComponent{};
};
