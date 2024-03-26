#pragma once

class CameraComponent;
class Transform;
class Shader;
class Mesh;

class MeshRenderer
{
public:
    
    void Render(const Mesh& mesh, const Transform& transform, Shader& shader) const;
};
