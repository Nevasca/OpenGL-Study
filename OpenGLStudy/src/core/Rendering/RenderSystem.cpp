#include "RenderSystem.h"
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Shader.h"
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

void RenderSystem::AddDirectionalLight(const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent)
{
    m_LightingSystem.AddDirectionalLight(directionalLightComponent);
}

void RenderSystem::Render(const CameraComponent& activeCamera)
{
    UpdateGlobalShaderUniforms(activeCamera);

#if INSTANCED_DRAW_ENABLED
    m_InstancedArray->Bind();

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
                assert(meshComponents[nextStartMeshIndex + i]->IsReadyToDraw());

                modelMatrices.emplace_back(meshComponents[nextStartMeshIndex + i]->GetOwnerTransform().GetMatrix());
            }

            m_InstancedArray->SetSubData(modelMatrices.data(), instancesToDraw * sizeof(glm::mat4));
            
            m_MeshRenderer.RenderInstanced(*meshComponents[nextStartMeshIndex]->GetMesh(), *meshComponents[nextStartMeshIndex]->GetShader(), instancesToDraw);
            
            remainingInstancesToDraw -= instancesToDraw;
            nextStartMeshIndex += instancesToDraw;
        }
    }

    m_InstancedArray->Unbind();

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

        m_LightingSystem.SetLightsFor(activeShader, activeCamera);
    }
}

void RenderSystem::CreateInstancedBuffer()
{
    m_InstancedArray = std::make_unique<InstancedArray>(nullptr, MAX_INSTANCED_AMOUNT_PER_CALL * sizeof(glm::mat4), true);
}

void RenderSystem::SetupInstancedMesh(const Mesh& mesh)
{
    m_InstancedArray->SetupInstancedAttributesFor(mesh.GetVertexArray());
}
