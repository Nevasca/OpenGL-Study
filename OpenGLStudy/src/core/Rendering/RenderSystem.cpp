#include "RenderSystem.h"
#include <glm/glm.hpp>

#include "Shader.h"
#include "core/Basics/Components/CameraComponent.h"
#include "core/Basics/Components/MeshComponent.h"
#include "core/GameObject/GameObject.h"

void RenderSystem::Shutdown()
{
    m_MeshComponents.clear();
    m_UniqueActiveShaders.clear();
}

void RenderSystem::AddMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
{
    std::shared_ptr<Shader> meshShader = meshComponent->GetShader();
    assert(meshShader);

    const unsigned int shaderId = meshShader->GetRendererID();
    if(m_UniqueActiveShaders.find(shaderId) == m_UniqueActiveShaders.end())
    {
        m_UniqueActiveShaders[shaderId] = meshShader;
    }

    if(m_MeshComponents.find(shaderId) == m_MeshComponents.end())
    {
        m_MeshComponents[shaderId] = {};
    }

    m_MeshComponents[shaderId].push_back(meshComponent);
}

void RenderSystem::Render(CameraComponent& activeCamera)
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

    for(auto& meshComponentPair : m_MeshComponents)
    {
        for(const auto& meshComponent : meshComponentPair.second)
        {
            assert(meshComponent->IsReadyToDraw());

            m_MeshRenderer.Render(*meshComponent->GetMesh(), meshComponent->GetOwner().GetTransform(), *meshComponent->GetShader());
        }
    }
}
