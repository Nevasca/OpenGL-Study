#include "RenderSystem.h"
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "core/Basics/Components/CameraComponent.h"
#include "core/Basics/Components/MeshComponent.h"
#include "core/GameObject/GameObject.h"

#define INSTANCED_DRAW_ENABLED 1;

RenderSystem::RenderSystem()
{
#if INSTANCED_DRAW_ENABLED
    CreateInstancedBuffer();
#endif
}

void RenderSystem::Shutdown()
{
    m_MeshComponents.clear();
    m_UniqueActiveShaders.clear();
}

void RenderSystem::AddMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
{
    assert(meshComponent->IsReadyToDraw());

    std::shared_ptr<Shader> meshShader = meshComponent->GetShader();

    const unsigned int shaderId = meshShader->GetRendererID();
    if(m_UniqueActiveShaders.find(shaderId) == m_UniqueActiveShaders.end())
    {
        m_UniqueActiveShaders[shaderId] = meshShader;
    }

    const std::shared_ptr<Mesh>& mesh = meshComponent->GetMesh();
    VertexArray& vao = mesh->GetVertexArray();
    const unsigned int vaoID = vao.GetRendererID();

    if(m_MeshComponents.find(vaoID) == m_MeshComponents.end())
    {
        m_MeshComponents[vaoID] = {};

#if INSTANCED_DRAW_ENABLED
        SetupInstancedMesh(*mesh);
#endif
    }

    m_MeshComponents[vaoID].push_back(meshComponent);
}

void RenderSystem::Render(const CameraComponent& activeCamera)
{
    UpdateGlobalShaderUniforms(activeCamera);

#if INSTANCED_DRAW_ENABLED
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_InstancedBufferId));

    // TODO: instead of render instanced by mesh, render by material and mesh
    // so we can have different shader uniforms applied for same mesh with different material
    for(auto& meshComponentPair : m_MeshComponents)
    {
        const std::vector<std::shared_ptr<MeshComponent>>& meshComponents = meshComponentPair.second;
        int remainingInstancesToDraw = static_cast<int>(meshComponents.size());
        int nextStartMeshIndex = 0;

        while(remainingInstancesToDraw > 0)
        {
            const int instancesToDraw = glm::min(remainingInstancesToDraw, MAX_INSTANCED_AMOUNT_PER_CALL);
            std::vector<glm::mat4> modelMatrices{};
            modelMatrices.reserve(instancesToDraw);

            for(int i = 0; i < instancesToDraw; i++)
            {
                modelMatrices.emplace_back(meshComponents[nextStartMeshIndex + i]->GetOwnerTransform().GetMatrix());
            }

            GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, instancesToDraw * sizeof(glm::mat4), modelMatrices.data()));
            
            m_MeshRenderer.RenderInstanced(*meshComponents[nextStartMeshIndex]->GetMesh(), *meshComponents[nextStartMeshIndex]->GetShader(), instancesToDraw);
            
            remainingInstancesToDraw -= instancesToDraw;
            nextStartMeshIndex += instancesToDraw;
        }
    }

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

#else
    for(auto& meshComponentPair : m_MeshComponents)
    {
        for(const auto& meshComponent : meshComponentPair.second)
        {
            assert(meshComponent->IsReadyToDraw());

            m_MeshRenderer.Render(*meshComponent->GetMesh(), meshComponent->GetOwner().GetTransform(), *meshComponent->GetShader());
        }
    }
#endif
}

void RenderSystem::UpdateGlobalShaderUniforms(const CameraComponent& activeCamera) const
{
    const glm::mat4 view = activeCamera.GetViewMatrix();
    const glm::mat4 proj = activeCamera.GetProjectionMatrix();

    for(auto& activeShaderPair : m_UniqueActiveShaders)
    {
        Shader& activeShader = *activeShaderPair.second;

        activeShader.Bind();
        activeShader.SetUniformMat4f("u_Proj", proj);
        activeShader.SetUniformMat4f("u_View", view);
    }
}

void RenderSystem::CreateInstancedBuffer()
{
    GLCall(glGenBuffers(1, &m_InstancedBufferId));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_InstancedBufferId));
    GLCall(glBufferData(GL_ARRAY_BUFFER, MAX_INSTANCED_AMOUNT_PER_CALL * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void RenderSystem::SetupInstancedMesh(const Mesh& mesh)
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_InstancedBufferId));

    mesh.GetVertexArray().Bind();

    // Set attribute pointers for model matrix (4 times vec4)
    GLCall(glEnableVertexAttribArray(3));
    GLCall(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0));
    GLCall(glEnableVertexAttribArray(4));
    GLCall(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4))));
    GLCall(glEnableVertexAttribArray(5));
    GLCall(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4))));
    GLCall(glEnableVertexAttribArray(6));
    GLCall(glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4))));

    //Instead of changing this attribute value on shader every new vertex (divisor 0)
    //we want it to change every new instance (divisor 1)
    GLCall(glVertexAttribDivisor(3, 1));
    GLCall(glVertexAttribDivisor(4, 1));
    GLCall(glVertexAttribDivisor(5, 1));
    GLCall(glVertexAttribDivisor(6, 1));

    mesh.GetVertexArray().Unbind();
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
