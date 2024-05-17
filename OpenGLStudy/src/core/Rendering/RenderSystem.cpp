#include "RenderSystem.h"
#include <glm/glm.hpp>

#include "Cubemap.h"
#include "Material.h"
#include "Mesh.h"
#include "Primitive.h"
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

    constexpr glm::vec4 defaultClearColor{0.2f, 0.2f, 0.2f, 1.f};
    SetClearColor(defaultClearColor);
    m_Device.EnableFaceCulling();

    SetupOutlineRendering();
    SetupSkybox();
}

void RenderSystem::Shutdown()
{
    m_OpaqueMeshComponentSet.Clear();
    m_TransparentMeshComponentSet.Clear();
    m_OpaqueOutlinedMeshComponentSet.Clear();
    m_TransparentOutlinedMeshComponentSet.Clear();

    m_UniqueActiveShaderSet.Clear();

    m_LightingSystem.Shutdown();
}

void RenderSystem::AddMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
{
    assert(meshComponent->IsReadyToDraw());

    Rendering::MeshComponentRenderSet& meshComponentSet = GetComponentRenderSetFor(meshComponent);
    meshComponentSet.Add(meshComponent, m_UniqueActiveShaderSet, *m_InstancedArray);
}

void RenderSystem::RemoveMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
{
    assert(meshComponent->IsReadyToDraw());
    
    Rendering::MeshComponentRenderSet& meshComponentSet = GetComponentRenderSetFor(meshComponent);
    meshComponentSet.Remove(meshComponent, m_UniqueActiveShaderSet);
}

void RenderSystem::AddOutlinedMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
{
    assert(meshComponent->IsReadyToDraw());

    Rendering::MeshComponentRenderSet& meshComponentSet = GetOutlinedComponentRenderSetFor(meshComponent);
    meshComponentSet.Add(meshComponent, m_UniqueActiveShaderSet, *m_InstancedArray);
}

void RenderSystem::RemoveOutlinedMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
{
    assert(meshComponent->IsReadyToDraw());

    Rendering::MeshComponentRenderSet& meshComponentSet = GetOutlinedComponentRenderSetFor(meshComponent);
    meshComponentSet.Remove(meshComponent, m_UniqueActiveShaderSet);
}

Rendering::MeshComponentRenderSet& RenderSystem::GetComponentRenderSetFor(const std::shared_ptr<MeshComponent>& meshComponent)
{
    const std::shared_ptr<Material>& material = meshComponent->GetMaterial();

    if(material->GetRenderingMode() == MaterialRenderingMode::Transparent)
    {
        return m_TransparentMeshComponentSet;
    }

    return m_OpaqueMeshComponentSet;
}

Rendering::MeshComponentRenderSet& RenderSystem::GetOutlinedComponentRenderSetFor(const std::shared_ptr<MeshComponent>& meshComponent)
{
    const std::shared_ptr<Material>& material = meshComponent->GetMaterial();

    if(material->GetRenderingMode() == MaterialRenderingMode::Transparent)
    {
        return m_TransparentOutlinedMeshComponentSet;
    }

    return m_OpaqueOutlinedMeshComponentSet;
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

    RenderWorld(activeCamera);
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

// Render distant objects first, used to render transparent objects
// best case scenario we have few different mesh/material with transparency, and we take advantage of instanced rendering
// worst case scenario we have lots of different mesh/material and their distance/placement make rendering almost as not using instanced rendering
void RenderSystem::RenderObjectsSortedByDistance(const Rendering::MeshComponentRenderSet& meshComponentSet, const glm::vec3& cameraPosition)
{
    if(meshComponentSet.IsEmpty())
    {
        return;
    }

    m_InstancedArray->Bind();

    std::multimap<float, Rendering::MeshComponentRenderElement> sortedObjects = meshComponentSet.GetMeshComponentsSortedByDistance(cameraPosition);

    auto& previousElement = sortedObjects.rbegin()->second;

    int totalPendingMeshesToRender = 0;
    std::vector<glm::mat4> modelMatrices{};
    
    for(auto it = sortedObjects.rbegin(); it != sortedObjects.rend(); ++it)
    {
        auto& renderElement = it->second;

        // If we reached max instanced amount per call or this is a different mesh/material
        // commit the render call with what was pending to render
        if(totalPendingMeshesToRender > MAX_INSTANCED_AMOUNT_PER_CALL
            || renderElement.VaoId != previousElement.VaoId
            || renderElement.MaterialId != previousElement.MaterialId)
        {
            m_InstancedArray->SetSubData(modelMatrices.data(), totalPendingMeshesToRender * sizeof(glm::mat4));
            
            m_MeshRenderer.RenderInstanced(
                *previousElement.MeshComponent->GetMesh(),
                *previousElement.MeshComponent->GetMaterial(),
                totalPendingMeshesToRender,
                m_WorldOverrideShader);

            totalPendingMeshesToRender = 0;
            modelMatrices.clear();
        }

        assert(renderElement.MeshComponent->IsReadyToDraw());
        modelMatrices.emplace_back(renderElement.MeshComponent->GetOwnerTransform().GetMatrix());
        
        previousElement = renderElement;
        totalPendingMeshesToRender++;
    }

    // Make sure to render pending meshes when we get out of loop
    if(totalPendingMeshesToRender > 0)
    {
        m_InstancedArray->SetSubData(modelMatrices.data(), totalPendingMeshesToRender * sizeof(glm::mat4));
            
        m_MeshRenderer.RenderInstanced(
            *previousElement.MeshComponent->GetMesh(),
            *previousElement.MeshComponent->GetMaterial(),
            totalPendingMeshesToRender,
            m_WorldOverrideShader);
    }

    m_InstancedArray->Unbind();
}

void RenderSystem::RenderWorld(const CameraComponent& activeCamera)
{
    m_Device.DisableStencilWrite();
    UpdateGlobalShaderUniforms(activeCamera);

    RenderObjects(m_OpaqueMeshComponentSet);
    RenderSkybox(activeCamera);
    RenderObjectsSortedByDistance(m_TransparentMeshComponentSet, activeCamera.GetOwnerPosition());
}

void RenderSystem::RenderSkybox(const CameraComponent& activeCamera)
{
    // Disable depth write so we draw skybox as background of all other objects
    // Also need to render back faces, since we are inside the sky cube
    //m_Device.DisableDepthWrite(); // Only when we render skybox first
    m_Device.SetDepthFunction(GL_LEQUAL); // Only when we render it last
    m_Device.SetCullingFaceFront();

    m_SkyboxMaterial->Bind();
    m_SkyboxMaterial->SetMat4("u_Proj", activeCamera.GetProjectionMatrix());

    // We use a view matrix with no translation so viewer can get move away from the skybox
    m_SkyboxMaterial->SetMat4("u_View", activeCamera.GetViewNoTranslationMatrix());

    m_MeshRenderer.Render(*m_SkyboxCube, *m_SkyboxMaterial);
    
    // m_Device.EnableDepthWrite();
    m_Device.SetDepthFunction(GL_LESS);
    m_Device.SetCullingFaceBack();
}

void RenderSystem::RenderOutlinedObjects(const CameraComponent& activeCamera)
{
    if(m_OpaqueOutlinedMeshComponentSet.IsEmpty() && m_TransparentOutlinedMeshComponentSet.IsEmpty())
    {
        return;        
    }

    m_Device.SetStencilFunction(GL_ALWAYS, 1.f, 0xFF);
    m_Device.EnableStencilWrite();

    RenderObjects(m_OpaqueOutlinedMeshComponentSet);
    RenderObjectsSortedByDistance(m_TransparentOutlinedMeshComponentSet, activeCamera.GetOwnerPosition());

    m_Device.SetStencilFunction(GL_NOTEQUAL, 1.f, 0xFF);
    m_Device.DisableStencilWrite();

    // On LearnOpenGL tutorial, we disable depth test to draw the outlined version
    // on top of possible shapes blocking it
    // the problem is for the floor on TestCore default world, it creates a wierd
    // effect by drawing the outline color on the cube upon it
    // m_Device.DisableDepthTest();

    constexpr glm::vec3 outlineThickness{0.06f};
    m_OpaqueOutlinedMeshComponentSet.OverrideAllObjectsScale(outlineThickness);
    m_TransparentOutlinedMeshComponentSet.OverrideAllObjectsScale(outlineThickness);

    std::shared_ptr<Shader> currentOverrideShader = m_WorldOverrideShader;
    SetOverrideShader(m_OutlineShader);
    UpdateGlobalShaderUniforms(activeCamera);

    RenderObjects(m_OpaqueOutlinedMeshComponentSet);
    RenderObjects(m_TransparentOutlinedMeshComponentSet);

    m_OpaqueOutlinedMeshComponentSet.OverrideAllObjectsScale(-outlineThickness);
    m_TransparentOutlinedMeshComponentSet.OverrideAllObjectsScale(-outlineThickness);
    SetOverrideShader(currentOverrideShader);

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

void RenderSystem::SetupSkybox()
{
    Rendering::CubemapLoadSettings skyCubemapSettings{
        "res/textures/skybox/right.jpg",
        "res/textures/skybox/left.jpg",
        "res/textures/skybox/top.jpg",
        "res/textures/skybox/bottom.jpg",
        "res/textures/skybox/back.jpg",
        "res/textures/skybox/front.jpg"
    };
    std::shared_ptr<Rendering::Cubemap> skyCubemap = ResourceManager::LoadCubemap(skyCubemapSettings, "C_Sky");

    const std::string SKYBOX_SHADER_NAME = "S_Skybox";
    ResourceManager::LoadShader("res/core/shaders/Skybox.glsl", SKYBOX_SHADER_NAME);
    m_SkyboxMaterial = ResourceManager::CreateMaterial("M_Skybox", SKYBOX_SHADER_NAME);
    m_SkyboxCube = Primitive::CreateSkyCube();

    m_SkyboxMaterial->SetCubemap("u_Skybox", skyCubemap, 0);
}
