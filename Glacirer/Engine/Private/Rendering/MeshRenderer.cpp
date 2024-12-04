#include "Rendering/MeshRenderer.h"

#include "Rendering/OpenGLCore.h"

#include "Rendering/IndexBuffer.h"
#include "Rendering/Material.h"
#include "Rendering/Mesh.h"
#include "Rendering/VertexArray.h"
#include "GameObject/Transform.h"

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

void MeshRenderer::Render(const Mesh& mesh, const Material& material) const
{
    material.Bind();

    mesh.GetVertexArray().Bind();

    const IndexBuffer& ibo = mesh.GetIndexBuffer();
    ibo.Bind();

    GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr));

    material.Unbind();
}

void MeshRenderer::RenderInstanced(const Mesh& mesh, const Material& material, int amount, const std::shared_ptr<Shader>& overrideShader) const
{
    if(overrideShader)
    {
        material.Bind(*overrideShader);
    }
    else
    {
        material.Bind();
    }

    mesh.GetVertexArray().Bind();
    const IndexBuffer& ibo = mesh.GetIndexBuffer();
    ibo.Bind();

    GLCall(glDrawElementsInstanced(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, nullptr, amount));
    // glEnable(GL_PROGRAM_POINT_SIZE);
    // GLCall(glDrawElementsInstanced(GL_POINTS, ibo.GetCount(), GL_UNSIGNED_INT, nullptr, amount));

    ibo.Unbind();
    mesh.GetVertexArray().Unbind();

    if(overrideShader)
    {
        material.Unbind(*overrideShader);
    }
    else
    {
        material.Unbind();
    }
}
