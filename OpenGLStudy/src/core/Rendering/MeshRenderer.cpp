#include "MeshRenderer.h"

#include "OpenGLCore.h"

#include "IndexBuffer.h"
#include "Material.h"
#include "Mesh.h"
#include "VertexArray.h"
#include "core/GameObject/Transform.h"

void MeshRenderer::Render(const Mesh& mesh, const Transform& transform, const Material& material) const
{
    material.Bind();
    material.SetMat4("u_Model", transform.GetMatrix());

    mesh.GetVertexArray().Bind();

    const IndexBuffer& ibo = mesh.GetIndexBuffer();
    ibo.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));

    material.Unbind();
}

void MeshRenderer::RenderInstanced(const Mesh& mesh, const Material& material, int amount) const
{
    material.Bind();

    mesh.GetVertexArray().Bind();
    const IndexBuffer& ibo = mesh.GetIndexBuffer();
    ibo.Bind();

    GLCall(glDrawElementsInstanced(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr, amount));

    ibo.Unbind();
    mesh.GetVertexArray().Unbind();

    material.Unbind();
}
