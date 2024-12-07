#include "Rendering/RenderSystem.h"
#include <glm/glm.hpp>

#include "Rendering/Cubemap.h"
#include "Rendering/Material.h"
#include "Rendering/Mesh.h"
#include "Rendering/Shader.h"
#include "ResourceManager.h"
#include "Screen.h"
#include "Basics/Components/CameraComponent.h"
#include "Basics/Components/MeshComponent.h"
#include "Basics/Components/PointLightComponent.h"
#include "Basics/Components/SkyboxComponent.h"
#include "Basics/Components/SpotLightComponent.h"
#include "GameObject/GameObject.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

RenderSystem::RenderSystem()
{
    CreateInstancedBuffer();
    CreateUniformBuffers();

    m_Device.EnableBlend();
    m_Device.SetBlendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_Device.EnableDepthTest();
    m_Device.EnableFaceCulling();
    m_Device.EnableMSAA(); // Seems we don't actually need to enable when using custom framebuffer
    
    Rendering::Resolution resolution = Screen::GetResolution();
    m_Device.SetViewportResolution(resolution);

    // TODO: add way to set how many samples to use
    constexpr unsigned int MSAA_SAMPLES = 4;
    m_MultisampleFramebuffer = std::make_unique<Framebuffer>(resolution, true, std::vector<TextureSettings>{}, MSAA_SAMPLES);
    m_IntermediateFramebuffer = std::make_unique<Framebuffer>(resolution, false, std::vector<TextureSettings>{});

    m_PostProcessingSystem.SetFramebuffer(*m_IntermediateFramebuffer);

    constexpr glm::vec4 defaultClearColor{0.1f, 0.1f, 0.1f, 1.f};
    SetClearColor(defaultClearColor);

    SetupOutlineRendering();
    SetupShadowRendering();
}

void RenderSystem::Setup()
{
    // Binding shadow map texture too early on constructor end ups getting unbound somewhere else internally 
    m_LightingSystem.Setup();

    SetupUniformsFor(*m_DirectionalDepthShader);
    SetupUniformsFor(*m_OmnidirectionalDepthShader);
    SetupUniformsFor(*m_OutlineShader);
}

void RenderSystem::Shutdown()
{
    m_Device.DisableMSAA();

    m_OpaqueMeshComponentSet.Clear();
    m_TransparentMeshComponentSet.Clear();
    m_OpaqueOutlinedMeshComponentSet.Clear();
    m_TransparentOutlinedMeshComponentSet.Clear();

    m_UniqueActiveShaderSet.Clear();

    m_LightingSystem.Shutdown();

    m_SkyboxComponent.reset();
}

void RenderSystem::AddMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
{
    assert(meshComponent->IsReadyToDraw());

    Rendering::MeshComponentRenderSet& meshComponentSet = GetComponentRenderSetFor(meshComponent);
    meshComponentSet.Add(meshComponent, *m_InstancedArray);

    std::shared_ptr<Shader> shader = meshComponent->GetShader();
    assert(shader);

    if(!m_UniqueActiveShaderSet.Contains(shader))
    {
        SetupUniformsFor(*shader);
    }

    m_UniqueActiveShaderSet.Add(shader);
}

void RenderSystem::RemoveMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
{
    assert(meshComponent->IsReadyToDraw());
    
    Rendering::MeshComponentRenderSet& meshComponentSet = GetComponentRenderSetFor(meshComponent);
    meshComponentSet.Remove(meshComponent);
    m_UniqueActiveShaderSet.Remove(meshComponent->GetShader());
}

void RenderSystem::AddOutlinedMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
{
    assert(meshComponent->IsReadyToDraw());

    Rendering::MeshComponentRenderSet& meshComponentSet = GetOutlinedComponentRenderSetFor(meshComponent);
    meshComponentSet.Add(meshComponent, *m_InstancedArray);

    std::shared_ptr<Shader> shader = meshComponent->GetShader();
    assert(shader);

    if(!m_UniqueActiveShaderSet.Contains(shader))
    {
        SetupUniformsFor(*shader);
    }

    m_UniqueActiveShaderSet.Add(shader);
}

void RenderSystem::RemoveOutlinedMeshComponent(const std::shared_ptr<MeshComponent>& meshComponent)
{
    assert(meshComponent->IsReadyToDraw());

    Rendering::MeshComponentRenderSet& meshComponentSet = GetOutlinedComponentRenderSetFor(meshComponent);
    meshComponentSet.Remove(meshComponent);
    m_UniqueActiveShaderSet.Remove(meshComponent->GetShader());
}

Rendering::MeshComponentRenderSet& RenderSystem::GetComponentRenderSetFor(const std::shared_ptr<MeshComponent>& meshComponent)
{
    const std::shared_ptr<Material>& material = meshComponent->GetMaterial();

    MaterialRenderingMode materialMode = material->GetRenderingMode();
    if(materialMode == MaterialRenderingMode::Transparent || materialMode == MaterialRenderingMode::AlphaCutout)
    {
        return m_TransparentMeshComponentSet;
    }

    return m_OpaqueMeshComponentSet;
}

Rendering::MeshComponentRenderSet& RenderSystem::GetOutlinedComponentRenderSetFor(const std::shared_ptr<MeshComponent>& meshComponent)
{
    const std::shared_ptr<Material>& material = meshComponent->GetMaterial();

    MaterialRenderingMode materialMode = material->GetRenderingMode();
    if(materialMode == MaterialRenderingMode::Transparent || materialMode == MaterialRenderingMode::AlphaCutout)
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

void RenderSystem::SetSkyboxComponent(const std::shared_ptr<SkyboxComponent>& skyboxComponent)
{
    assert(skyboxComponent && skyboxComponent->IsReadyToDraw());

    m_SkyboxComponent = skyboxComponent;
    const std::shared_ptr<Material>& skyboxMaterial = m_SkyboxComponent->GetMaterial();
    const std::shared_ptr<Shader>& skyboxShader = skyboxMaterial->GetShader();
    
    skyboxShader->Bind();
    skyboxShader->SetUniform1i("u_Skybox", SKYBOX_CUBEMAP_SLOT);
    skyboxShader->Unbind();

    const std::shared_ptr<Rendering::Cubemap>& skyboxCubemap = skyboxComponent->GetCubemap();
    skyboxCubemap->Bind(SKYBOX_CUBEMAP_SLOT);
}

void RenderSystem::RemoveSkyboxComponent(const std::shared_ptr<SkyboxComponent>& skyboxComponent)
{
    if(m_SkyboxComponent != skyboxComponent)
    {
        return;
    }

    m_SkyboxComponent = nullptr;
}

void RenderSystem::Render(const CameraComponent& activeCamera)
{
    UpdateGlobalShaderUniforms(activeCamera);

    RenderShadowPass(activeCamera);

    m_MultisampleFramebuffer->BindAndClear();

    RenderWorld(activeCamera);
    RenderOutlinedObjects(activeCamera);

    // Blit (resolve) multisample framebuffer so we can sample the result color texture on post-processing
    m_MultisampleFramebuffer->BindAsReadOnly();
    m_IntermediateFramebuffer->BindAsWriteOnly();
    m_MultisampleFramebuffer->ResolveMultisampleImage(m_IntermediateFramebuffer->GetResolution());
    m_MultisampleFramebuffer->Unbind();

    // If we weren't gamma correction via shader and wanted to correct automatically with OpenGL
    // m_Device.EnableGammaCorrection();
    m_PostProcessingSystem.RenderToScreen();
    // m_Device.DisableGammaCorrection();
}

void RenderSystem::SetOverrideShader(const std::shared_ptr<Shader>& overrideShader, bool bSetupUniforms)
{
    m_WorldOverrideShader = overrideShader;

    if(m_WorldOverrideShader && bSetupUniforms)
    {
        SetupUniformsFor(*m_WorldOverrideShader);
    }
}

void RenderSystem::UpdateGlobalShaderUniforms(const CameraComponent& activeCamera)
{
    UpdateCameraMatricesShaderUniforms(activeCamera);

    m_CameraUniformBuffer->Bind();
    float cameraParams[2] { activeCamera.GetNearPlane(), activeCamera.GetFarPlane() };
    m_CameraUniformBuffer->SetSubData(cameraParams, sizeof(cameraParams));
    m_CameraUniformBuffer->Unbind();

    m_LightingSystem.UpdateLightingUniformBuffer(activeCamera);
}

void RenderSystem::UpdateCameraMatricesShaderUniforms(const CameraComponent& activeCamera)
{
    const glm::mat4 view = activeCamera.GetViewMatrix();
    const glm::mat4 proj = activeCamera.GetProjectionMatrix();

    m_MatricesUniformBuffer->Bind();
    glm::mat4 matrices[2] { proj, view };
    m_MatricesUniformBuffer->SetSubData(matrices, sizeof(matrices));
    m_MatricesUniformBuffer->Unbind();
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

    RenderObjects(m_OpaqueMeshComponentSet);
    RenderSkybox(activeCamera);
    RenderObjectsSortedByDistance(m_TransparentMeshComponentSet, activeCamera.GetOwnerPosition());
}

void RenderSystem::RenderShadowPass(const CameraComponent& activeCamera)
{
    std::shared_ptr<Shader> previousOverrideShader = m_WorldOverrideShader;

    RenderDirectionalShadowPass();
    RenderPointShadowPass();
    RenderSpotShadowPass();

    UpdateCameraMatricesShaderUniforms(activeCamera);
    SetOverrideShader(previousOverrideShader, false);
    m_Device.SetViewportResolution(Screen::GetResolution());
}

void RenderSystem::RenderDirectionalShadowPass()
{
    int totalActiveDirectionalLights = m_LightingSystem.GetTotalActiveDirectionalLights();

    if(totalActiveDirectionalLights == 0)
    {
        return;
    }

    SetOverrideShader(m_DirectionalDepthShader, false);

    for(int i = 0; i < totalActiveDirectionalLights; i++)
    {
        const DirectionalLightComponent& directionalLight = m_LightingSystem.GetDirectionalLight(i);

        if(!directionalLight.IsCastShadowEnabled())
        {
            continue;
        }

        const Framebuffer& shadowMapBuffer = m_LightingSystem.GetDirectionalShadowMapFramebuffer(i);

        m_Device.SetViewportResolution(shadowMapBuffer.GetResolution());
        shadowMapBuffer.BindAndClear();

        glm::vec3 lightPosition = directionalLight.GetOwnerPosition();
        const glm::mat4 view = directionalLight.GetViewMatrix();
        const glm::mat4 proj = directionalLight.GetProjectionMatrix();

        m_MatricesUniformBuffer->Bind();
        glm::mat4 matrices[2] { proj, view };
        m_MatricesUniformBuffer->SetSubData(matrices, sizeof(matrices));
        m_MatricesUniformBuffer->Unbind();

        RenderWorldForShadowPass(lightPosition);
        
        shadowMapBuffer.Unbind();
    }
}

void RenderSystem::RenderPointShadowPass()
{
    int totalActivePointLights = m_LightingSystem.GetTotalActivePointLights();

    if(totalActivePointLights == 0)
    {
        return;
    }

    SetOverrideShader(m_OmnidirectionalDepthShader, false);

    for(int i = 0; i < totalActivePointLights; i++)
    {
        const PointLightComponent& pointLight = m_LightingSystem.GetPointLight(i);

        if(!pointLight.IsCastShadowEnabled())
        {
            continue;
        }

        const Framebuffer& shadowMapBuffer = m_LightingSystem.GetPointLightShadowMapFramebuffer(i);

        m_Device.SetViewportResolution(shadowMapBuffer.GetResolution());
        shadowMapBuffer.BindAndClear();

        m_OmnidirectionalDepthShader->Bind();
        m_OmnidirectionalDepthShader->SetUniform1i("u_LightIndex", i);
        m_OmnidirectionalDepthShader->Unbind();

        RenderWorldForShadowPass(pointLight.GetPosition());
        
        shadowMapBuffer.Unbind();
    }
}

void RenderSystem::RenderSpotShadowPass()
{
    int totalActiveSpotLights = m_LightingSystem.GetTotalActiveSpotLights();

    if(totalActiveSpotLights == 0)
    {
        return;
    }

    SetOverrideShader(m_DirectionalDepthShader, false);

    for(int i = 0; i < totalActiveSpotLights; i++)
    {
        const SpotLightComponent& spotLightComponent = m_LightingSystem.GetSpotLight(i);

        if(!spotLightComponent.IsCastShadowEnabled())
        {
            continue;
        }

        const Framebuffer& shadowMapBuffer = m_LightingSystem.GetSpotLightShadowMapFramebuffer(i);

        Rendering::Resolution shadowMapResolution = shadowMapBuffer.GetResolution();
        m_Device.SetViewportResolution(shadowMapResolution);
        shadowMapBuffer.BindAndClear();

        glm::vec3 lightPosition = spotLightComponent.GetOwnerPosition();
        const glm::mat4 view = spotLightComponent.GetViewMatrix();
        const glm::mat4 proj = spotLightComponent.GetProjectionMatrix(shadowMapResolution);

        m_MatricesUniformBuffer->Bind();
        glm::mat4 matrices[2] { proj, view };
        m_MatricesUniformBuffer->SetSubData(matrices, sizeof(matrices));
        m_MatricesUniformBuffer->Unbind();

        RenderWorldForShadowPass(lightPosition);
        
        shadowMapBuffer.Unbind();
    }
}

void RenderSystem::RenderWorldForShadowPass(const glm::vec3& lightPosition)
{
    bool bPreviousFaceCullingEnabled = m_Device.IsFaceCullingEnabled();
    if(!bPreviousFaceCullingEnabled)
    {
        m_Device.EnableFaceCulling();
    }

    // Set front face culling to fix petter panning shadow
    m_Device.SetCullingFaceFront();
    RenderObjects(m_OpaqueMeshComponentSet);

    // TODO: temp fix for casting shadow for one sided transparent object
    // a better solution would be having a render set for objects that need to cast shadow from both sides (like a DoubleSided flag on MeshComponent or Material) 
    m_Device.DisableFaceCulling();
    RenderObjectsSortedByDistance(m_TransparentMeshComponentSet, lightPosition);
    m_Device.EnableFaceCulling();
    
    m_Device.SetCullingFaceFront();
    RenderObjects(m_OpaqueOutlinedMeshComponentSet);
    m_Device.SetCullingFaceBack();

    m_Device.DisableFaceCulling();
    RenderObjectsSortedByDistance(m_TransparentOutlinedMeshComponentSet, lightPosition);

    if(bPreviousFaceCullingEnabled)
    {
        m_Device.EnableFaceCulling();
    }
}

void RenderSystem::RenderSkybox(const CameraComponent& activeCamera)
{
    if(!IsSkyboxActive())
    {
        return;
    }

    // Disable depth write so we draw skybox as background of all other objects
    // Also need to render back faces, since we are inside the sky cube
    //m_Device.DisableDepthWrite(); // Only when we render skybox first
    m_Device.SetDepthFunction(GL_LEQUAL); // Only when we render it last
    m_Device.SetCullingFaceFront();

    const std::shared_ptr<Material>& skyboxMaterial = m_SkyboxComponent->GetMaterial();
    skyboxMaterial->Bind();
    skyboxMaterial->SetMat4("u_Proj", activeCamera.GetProjectionMatrix());

    // We use a view matrix with no translation so viewer can get move away from the skybox
    skyboxMaterial->SetMat4("u_View", activeCamera.GetViewNoTranslationMatrix());

    m_MeshRenderer.Render(*m_SkyboxComponent->GetMesh(), *skyboxMaterial);
    
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
    SetOverrideShader(m_OutlineShader, false);

    RenderObjects(m_OpaqueOutlinedMeshComponentSet);
    RenderObjects(m_TransparentOutlinedMeshComponentSet);

    m_OpaqueOutlinedMeshComponentSet.OverrideAllObjectsScale(-outlineThickness);
    m_TransparentOutlinedMeshComponentSet.OverrideAllObjectsScale(-outlineThickness);
    SetOverrideShader(currentOverrideShader, false);

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

void RenderSystem::CreateUniformBuffers()
{
    constexpr unsigned int UNIFORM_MATRICES_BINDING_INDEX = 0;
    constexpr unsigned int UNIFORM_CAMERA_BINDING_INDEX = 1;

    m_MatricesUniformBuffer = std::make_unique<Rendering::UniformBuffer>(nullptr,
        2 * sizeof(glm::mat4),
        UNIFORM_MATRICES_BINDING_INDEX,
        "Matrices",
        true);

    m_CameraUniformBuffer = std::make_unique<Rendering::UniformBuffer>(nullptr,
        2 * sizeof(float),
        UNIFORM_CAMERA_BINDING_INDEX,
        "Camera");
}

void RenderSystem::SetupUniformsFor(Shader& shader) const
{
    m_MatricesUniformBuffer->SetBindingIndexFor(shader);
    m_CameraUniformBuffer->SetBindingIndexFor(shader);

    shader.Bind();
    shader.SetUniform1i("u_Skybox", SKYBOX_CUBEMAP_SLOT);
    shader.Unbind();

    m_LightingSystem.SetupUniformsFor(shader);
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

bool RenderSystem::IsSkyboxActive() const
{
    return m_SkyboxComponent && m_SkyboxComponent->IsReadyToDraw() && bIsSkyboxEnabled;
}

void RenderSystem::SetupShadowRendering()
{
    m_DirectionalDepthShader = ResourceManager::LoadShader("res/core/shaders/AlphaSimpleDepth.glsl", "AlphaSimpleDepth");
    m_OmnidirectionalDepthShader = ResourceManager::LoadShader("res/core/shaders/OmnidirectionalDepth.glsl", "OminidirectionalDepth");
}
