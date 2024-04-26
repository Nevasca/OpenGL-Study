#pragma once

class Material;
class CameraComponent;
class Transform;
class Mesh;

class MeshRenderer
{
public:
    
    void Render(const Mesh& mesh, const Transform& transform, const Material& material) const;
    void Render(const Mesh& mesh, const Material& material) const;
    void RenderInstanced(const Mesh& mesh, const Material& material, int amount) const;
};
