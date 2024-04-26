#include "RenderSystem.h"
#include <glm/glm.hpp>

#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "core/Screen.h"
#include "core/Basics/Components/CameraComponent.h"
#include "core/Basics/Components/MeshComponent.h"
#include "core/GameObject/GameObject.h"

RenderSystem::RenderSystem()
{
    CreateInstancedBuffer();

    m_Framebuffer = std::make_unique<Framebuffer>(Screen::GetWidth(), Screen::GetHeight(), true, std::vector<TextureSettings>{});
    m_PostProcessingSystem.SetFramebuffer(*m_Framebuffer);
}

void RenderSystem::Shutdown()
{
    m_MeshComponents.clear();
    m_UniqueActiveShaders.clear();

    m_LightingSystem.Shutdown();
}

void RenderSystem::AddMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
{
    assert(meshComponent->IsReadyToDraw());

    const std::shared_ptr<Material>& meshMaterial = meshComponent->GetMaterial();
    const unsigned int materialId = meshMaterial->GetId();
    
    const std::shared_ptr<Shader>& meshShader = meshMaterial->GetShader();
    const unsigned int shaderId = meshShader->GetRendererID();

    if(m_UniqueActiveShaders.find(shaderId) == m_UniqueActiveShaders.end())
    {
        ActiveShader activeShader{};
        activeShader.Shader = meshShader;
        
        m_UniqueActiveShaders[shaderId] = activeShader;
    }

    m_UniqueActiveShaders[shaderId].UsageCount++;

    const std::shared_ptr<Mesh>& mesh = meshComponent->GetMesh();
    VertexArray& vao = mesh->GetVertexArray();
    const unsigned int vaoID = vao.GetRendererID();

    if(m_MeshComponents.find(vaoID) == m_MeshComponents.end())
    {
        m_MeshComponents[vaoID] = {};

        SetupInstancedMesh(*mesh);
    }

    if(m_MeshComponents[vaoID].find(materialId) == m_MeshComponents[vaoID].end())
    {
        m_MeshComponents[vaoID][materialId] = {};
    }

    m_MeshComponents[vaoID][materialId].push_back(meshComponent);
}

void RenderSystem::RemoveMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
{
    assert(meshComponent->IsReadyToDraw());

    const std::shared_ptr<Material>& meshMaterial = meshComponent->GetMaterial();
    const unsigned int materialId = meshMaterial->GetId();
    
    const std::shared_ptr<Shader>& meshShader = meshMaterial->GetShader();
    const unsigned int shaderId = meshShader->GetRendererID();

    m_UniqueActiveShaders[shaderId].UsageCount--;

    if(m_UniqueActiveShaders[shaderId].UsageCount <= 0)
    {
        m_UniqueActiveShaders.erase(shaderId);
    }

    const std::shared_ptr<Mesh>& mesh = meshComponent->GetMesh();
    VertexArray& vao = mesh->GetVertexArray();
    const unsigned int vaoID = vao.GetRendererID();

    std::vector<std::shared_ptr<MeshComponent>>& components = m_MeshComponents[vaoID][materialId];
    components.erase(std::remove(components.begin(), components.end(), meshComponent), components.end());
}

void RenderSystem::AddDirectionalLight(const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent)
{
    m_LightingSystem.AddDirectionalLight(directionalLightComponent);
}

void RenderSystem::AddPointLight(const std::shared_ptr<PointLightComponent>& pointLightComponent)
{
    m_LightingSystem.AddPointLight(pointLightComponent);
}

void RenderSystem::AddSpotLight(const std::shared_ptr<SpotLightComponent>& spotLightComponent)
{
    m_LightingSystem.AddSpotLight(spotLightComponent);
}

void RenderSystem::SetPostProcessingComponent(const std::shared_ptr<PostProcessingComponent>& postProcessingComponent)
{
    m_PostProcessingSystem.SetPostProcessingComponent(postProcessingComponent);
}

void RenderSystem::RemovePostProcessingComponent(const std::shared_ptr<PostProcessingComponent>& postProcessingComponent)
{
    m_PostProcessingSystem.RemovePostProcessingComponent(postProcessingComponent);
}

void RenderSystem::Render(const CameraComponent& activeCamera)
{
    UpdateGlobalShaderUniforms(activeCamera);

    m_Framebuffer->BindAndClear();
    RenderWorldObjects();
    m_Framebuffer->Unbind();

    m_PostProcessingSystem.RenderToScreen();
}

void RenderSystem::UpdateGlobalShaderUniforms(const CameraComponent& activeCamera) const
{
    const glm::mat4 view = activeCamera.GetViewMatrix();
    const glm::mat4 proj = activeCamera.GetProjectionMatrix();

    for(auto& activeShaderPair : m_UniqueActiveShaders)
    {
        Shader& activeShader = *activeShaderPair.second.Shader;

        activeShader.Bind();
        activeShader.SetUniformMat4f("u_Proj", proj);
        activeShader.SetUniformMat4f("u_View", view);

        m_LightingSystem.SetLightsFor(activeShader, activeCamera);
    }
}

void RenderSystem::RenderWorldObjects()
{
    m_InstancedArray->Bind();

    // Foreach unique VAO
    for(auto& meshMappingPair : m_MeshComponents)
    {
        // Foreach material
        for(auto& meshComponentPair : meshMappingPair.second)
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
                
                m_MeshRenderer.RenderInstanced(*meshComponents[nextStartMeshIndex]->GetMesh(), *meshComponents[nextStartMeshIndex]->GetMaterial(), instancesToDraw);
                
                remainingInstancesToDraw -= instancesToDraw;
                nextStartMeshIndex += instancesToDraw;
            }
        }
    }

    m_InstancedArray->Unbind();
}

void RenderSystem::CreateInstancedBuffer()
{
    VertexBufferLayout layout{};

    //Instead of changing this attribute value on shader every new vertex (divisor 0)
    //we want it to change every new instance (divisor 1)
    layout.PushMat4(1);

    m_InstancedArray = std::make_unique<InstancedArray>(nullptr,
        MAX_INSTANCED_AMOUNT_PER_CALL * layout.GetStride(),
        true,
        std::move(layout));
}

void RenderSystem::SetupInstancedMesh(const Mesh& mesh)
{
    m_InstancedArray->SetupInstancedAttributesFor(mesh.GetVertexArray());
}
