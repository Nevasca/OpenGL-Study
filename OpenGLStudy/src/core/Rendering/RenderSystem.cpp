#include "RenderSystem.h"
#include <glm/glm.hpp>

#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "core/ResourceManager.h"
#include "core/Screen.h"
#include "core/Basics/Components/CameraComponent.h"
#include "core/Basics/Components/MeshComponent.h"
#include "core/GameObject/GameObject.h"

RenderSystem::RenderSystem()
{
    CreateInstancedBuffer();

    m_Framebuffer = std::make_unique<Framebuffer>(Screen::GetWidth(), Screen::GetHeight(), true, std::vector<TextureSettings>{});
    m_PostProcessingSystem.SetFramebuffer(*m_Framebuffer);

    SetupOutlineRendering();
}

void RenderSystem::Shutdown()
{
    m_MeshComponentSet.Clear();
    m_UniqueActiveShaderSet.Clear();

    m_LightingSystem.Shutdown();
}

void RenderSystem::AddMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
{
    assert(meshComponent->IsReadyToDraw());
    m_MeshComponentSet.Add(meshComponent, m_UniqueActiveShaderSet, *m_InstancedArray);
}

void RenderSystem::RemoveMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
{
    assert(meshComponent->IsReadyToDraw());
    m_MeshComponentSet.Remove(meshComponent, m_UniqueActiveShaderSet);
}

void RenderSystem::AddOutlinedMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
{
    assert(meshComponent->IsReadyToDraw());
    m_OutlinedMeshComponentSet.Add(meshComponent, m_UniqueActiveShaderSet, *m_InstancedArray);
}

void RenderSystem::RemoveOutlinedMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
{
    assert(meshComponent->IsReadyToDraw());
    m_OutlinedMeshComponentSet.Remove(meshComponent, m_UniqueActiveShaderSet);
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
    m_Framebuffer->BindAndClear();

    RenderWorldObjects(activeCamera);
    RenderOutlinedObjects(activeCamera);
    
    m_Framebuffer->Unbind();

    m_PostProcessingSystem.RenderToScreen();
}

void RenderSystem::UpdateGlobalShaderUniforms(const CameraComponent& activeCamera)
{
    const glm::mat4 view = activeCamera.GetViewMatrix();
    const glm::mat4 proj = activeCamera.GetProjectionMatrix();

    // If we have an override shader to use for all objects
    // just update it and ignore world active shaders
    if(m_WorldOverrideShader)
    {
        m_WorldOverrideShader->Bind();
        m_WorldOverrideShader->SetUniformMat4f("u_Proj", proj);
        m_WorldOverrideShader->SetUniformMat4f("u_View", view);
        m_WorldOverrideShader->SetUniform1f("u_NearPlane", activeCamera.GetNearPlane());
        m_WorldOverrideShader->SetUniform1f("u_FarPlane", activeCamera.GetFarPlane());

        m_LightingSystem.SetLightsFor(*m_WorldOverrideShader, activeCamera);

        return;    
    }
    
    for(auto& activeShaderPair : m_UniqueActiveShaderSet.GetShaders())
    {
        Shader& activeShader = *activeShaderPair.second.Shader;

        activeShader.Bind();
        activeShader.SetUniformMat4f("u_Proj", proj);
        activeShader.SetUniformMat4f("u_View", view);
        activeShader.SetUniform1f("u_NearPlane", activeCamera.GetNearPlane());
        activeShader.SetUniform1f("u_FarPlane", activeCamera.GetFarPlane());

        m_LightingSystem.SetLightsFor(activeShader, activeCamera);
    }
}

void RenderSystem::RenderObjects(const Rendering::MeshComponentRenderSet& meshComponentSet)
{
    m_InstancedArray->Bind();

    // Foreach unique VAO
    for(auto& meshMappingPair : meshComponentSet.GetMeshComponents())
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
                
                m_MeshRenderer.RenderInstanced(
                    *meshComponents[nextStartMeshIndex]->GetMesh(),
                    *meshComponents[nextStartMeshIndex]->GetMaterial(),
                    instancesToDraw,
                    m_WorldOverrideShader);
                
                remainingInstancesToDraw -= instancesToDraw;
                nextStartMeshIndex += instancesToDraw;
            }
        }
    }

    m_InstancedArray->Unbind();
}

void RenderSystem::RenderWorldObjects(const CameraComponent& activeCamera)
{
    m_Device.DisableStencilWrite();
    UpdateGlobalShaderUniforms(activeCamera);

    RenderObjects(m_MeshComponentSet);
}

void RenderSystem::RenderOutlinedObjects(const CameraComponent& activeCamera)
{
    if(m_OutlinedMeshComponentSet.IsEmpty())
    {
        return;        
    }

    m_Device.SetStencilFunction(GL_ALWAYS, 1.f, 0xFF);
    m_Device.EnableStencilWrite();

    RenderObjects(m_OutlinedMeshComponentSet);

    m_Device.SetStencilFunction(GL_NOTEQUAL, 1.f, 0xFF);
    m_Device.DisableStencilWrite();

    // On LearnOpenGL tutorial, we disable depth test to draw the outlined version
    // on top of possible shapes blocking it
    // the problem is for the floor on TestCore default world, it creates a wierd
    // effect by drawing the outline color on the cube upon it
    // m_Device.DisableDepthTest();

    constexpr glm::vec3 outlineThickness{0.06f};
    m_OutlinedMeshComponentSet.OverrideAllObjectsScale(outlineThickness);
    SetOverrideShader(m_OutlineShader);
    UpdateGlobalShaderUniforms(activeCamera);

    RenderObjects(m_OutlinedMeshComponentSet);

    m_OutlinedMeshComponentSet.OverrideAllObjectsScale(-outlineThickness);
    SetOverrideShader(nullptr);

    m_Device.EnableStencilWrite();
    m_Device.SetStencilFunction(GL_ALWAYS, 1.f, 0xFF);
    // m_Device.EnableDepthTest();
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

void RenderSystem::SetupOutlineRendering()
{
    m_Device.EnableStencilTest();
    m_Device.SetStencilOperation(GL_KEEP, GL_KEEP, GL_REPLACE);

    m_OutlineShader = ResourceManager::LoadShader("res/core/shaders/Outline.glsl", "Outline");

    constexpr glm::vec4 OUTLINE_COLOR = glm::vec4{1.f, 0.576f, 0.f, 1.f};

    m_OutlineShader->Bind();
    m_OutlineShader->SetUniform4f("u_OutlineColor", OUTLINE_COLOR);
    m_OutlineShader->Unbind();
}
