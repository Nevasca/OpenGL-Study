#include "MeshRenderer.h"

#include "Core.h"

#include "IndexBuffer.h"
#include "Mesh.h"
#include "Shader.h"
#include "VertexArray.h"
#include "core/GameObject/Transform.h"

void MeshRenderer::Render(const Mesh& mesh, const Transform& transform, Shader& shader) const
{
    shader.Bind();
    shader.SetUniformMat4f("u_Model", transform.GetMatrix());

    mesh.GetVertexArray().Bind();

    const IndexBuffer& ibo = mesh.GetIndexBuffer();
    ibo.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void MeshRenderer::RenderInstanced(const Mesh& mesh, Shader& shader, int amount) const
{
    shader.Bind();

    mesh.GetVertexArray().Bind();
    const IndexBuffer& ibo = mesh.GetIndexBuffer();
    ibo.Bind();

    GLCall(glDrawElementsInstanced(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr, amount));

    ibo.Unbind();
    mesh.GetVertexArray().Unbind();
}
