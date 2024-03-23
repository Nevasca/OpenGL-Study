#pragma once

#include "core/GameObject/Component.h"

class Shader;
class Mesh;

class MeshComponent : public Component
{
    GENERATE_COMPONENT_BODY(MeshComponent)

public:

    void Initialize() override;
    void SetMesh(const std::shared_ptr<Mesh>& mesh);
    void SetShader(const std::shared_ptr<Shader>& shader);
    
    bool IsReadyToDraw() const { return m_Mesh != nullptr && m_Shader != nullptr; }
    const std::shared_ptr<Mesh>& GetMesh() const { return m_Mesh; }
    const std::shared_ptr<Shader>& GetShader() const { return m_Shader; }

private:

    std::shared_ptr<Mesh> m_Mesh;
    std::shared_ptr<Shader> m_Shader;
};
