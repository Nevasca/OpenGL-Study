#pragma once
#include "glm/fwd.hpp"

class CameraComponent;
class Transform;
class Shader;
class Mesh;

class MeshRenderer
{
public:
    
    void Render(const Mesh& mesh, const Transform& transform, const glm::mat4& proj, const glm::mat4& view, Shader& shader) const;
};
