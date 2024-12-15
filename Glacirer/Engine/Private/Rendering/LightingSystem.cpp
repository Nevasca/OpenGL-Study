#include "Rendering/LightingSystem.h"

#include <iostream>

#include "Rendering/Shader.h"
#include "Basics/Components/CameraComponent.h"
#include "Basics/Components/DirectionalLightComponent.h"
#include "Basics/Components/PointLightComponent.h"
#include "Basics/Components/SpotLightComponent.h"

namespace Glacirer
{
    namespace Rendering
    {
        LightingSystem::LightingSystem()
        {
            CreateUniformBuffers();
            CreateShadowMaps();
        }

        void LightingSystem::Shutdown()
        {
            m_DirectionalLights.clear();
            m_PointLights.clear();
            m_SpotLights.clear();

            m_GeneralUniformBuffer.reset();
            m_DirectionalUniformBuffer.reset();
            m_PointUniformBuffer.reset();
            m_SpotsUniformBuffer.reset();
            m_DirectionalMatrixUniformBuffer.reset();
            m_PointLightMatricesUniformBuffer.reset();

            UnbindShadowMapTextures();

            m_DirectionalShadowMapBuffers.clear();
            m_PointLightShadowMapBuffers.clear();
            m_SpotLightShadowMapBuffers.clear();
        }

        void LightingSystem::AddDirectionalLight(const std::shared_ptr<DirectionalLightComponent>& directionalLightComponent)
        {
            m_DirectionalLights.push_back(directionalLightComponent);
            m_TotalActiveDirectionalLights = std::min(static_cast<int>(m_DirectionalLights.size()), MAX_DIRECTIONAL_LIGHTS);
    
            if(m_DirectionalLights.size() > MAX_DIRECTIONAL_LIGHTS)
            {
                // TODO: replace for log class when implemented
                std::cout << "Max directional lights reached. Last added directional light won't affect world\n";
            }

            int lastActiveLightIndex = m_TotalActiveDirectionalLights - 1;
            CreateDirectionalLightShadowMapFor(lastActiveLightIndex);
        }

        void LightingSystem::AddPointLight(const std::shared_ptr<PointLightComponent>& pointLightComponent)
        {
            m_PointLights.push_back(pointLightComponent);
            m_TotalActivePointLights = std::min(static_cast<int>(m_PointLights.size()), MAX_POINT_LIGHTS);

            if(m_PointLights.size() > MAX_POINT_LIGHTS)
            {
                // TODO: replace for log class when implemented
                std::cout << "Max point lights reached. Last added point light won't affect world\n";
                return;
            }

            int lastActiveLightIndex = m_TotalActivePointLights - 1;
            CreatePointLightShadowMapFor(lastActiveLightIndex);
        }

        void LightingSystem::AddSpotLight(const std::shared_ptr<SpotLightComponent>& spotLightComponent)
        {
            m_SpotLights.push_back(spotLightComponent);
            m_TotalActiveSpotLights = std::min(static_cast<int>(m_SpotLights.size()), MAX_SPOT_LIGHTS);

            if(m_SpotLights.size() > MAX_SPOT_LIGHTS)
            {
                // TODO: replace for log class when implemented
                std::cout << "Max spot lights reached. Last added spot light won't affect world\n";
            }

            int lastActiveLightIndex = m_TotalActiveSpotLights - 1;
            CreateSportLightShadowMapFor(lastActiveLightIndex);
        }

        void LightingSystem::SetupUniformsFor(Shader& shader) const
        {
            m_GeneralUniformBuffer->SetBindingIndexFor(shader);
            m_DirectionalUniformBuffer->SetBindingIndexFor(shader);
            m_PointUniformBuffer->SetBindingIndexFor(shader);
            m_SpotsUniformBuffer->SetBindingIndexFor(shader);
            m_DirectionalMatrixUniformBuffer->SetBindingIndexFor(shader);
            m_PointLightMatricesUniformBuffer->SetBindingIndexFor(shader);
            m_SpotLightMatricesUniformBuffer->SetBindingIndexFor(shader);

            shader.Bind();

            for(int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
            {
                std::string uniformName = "u_DirectionalLightShadowMaps[" + std::to_string(i) + "]";
                shader.SetUniform1i(uniformName, DIRECTIONAL_SHADOW_MAP_START_SLOT + i);
            }

            for(int i = 0; i < MAX_POINT_LIGHTS; i++)
            {
                std::string uniformName = "u_PointLightShadowMaps[" + std::to_string(i) + "]";
                shader.SetUniform1i(uniformName, POINT_SHADOW_MAP_START_SLOT + i);
            }

            for(int i = 0; i < MAX_SPOT_LIGHTS; i++)
            {
                std::string uniformName = "u_SpotLightShadowMaps[" + std::to_string(i) + "]";
                shader.SetUniform1i(uniformName, SPOT_SHADOW_MAP_START_SLOT + i);
            }

            shader.Unbind();
        }

        void LightingSystem::UpdateLightingUniformBuffer(const CameraComponent& activeCamera)
        {
            // TODO we could try testing performance if light is dirty and skip updating uniform if not

            m_GeneralShaderData.ViewPosition = activeCamera.GetOwnerPosition();
            m_GeneralShaderData.AmbientLight.Color = m_AmbientLightColor;

            m_GeneralShaderData.TotalDirectionalLights = m_TotalActiveDirectionalLights;
    
            for(int i = 0; i < m_TotalActiveDirectionalLights; i++)
            {
                auto& directionalLight = m_DirectionalLights[i];
    
                DirectionalLightShaderData directionalLightShaderData;
                directionalLightShaderData.Intensity = directionalLight->GetIntensity();
                directionalLightShaderData.Bias = directionalLight->GetBias();
                directionalLightShaderData.NormalBias = directionalLight->GetNormalBias();
                directionalLightShaderData.Direction = directionalLight->GetDirection();
                directionalLightShaderData.Diffuse = directionalLight->GetColor();
                directionalLightShaderData.Specular = m_DefaultSpecularColor;
                directionalLightShaderData.CastShadow = directionalLight->IsCastShadowEnabled() ? 1 : 0;
        
                m_DirectionalsShaderData[i] = directionalLightShaderData;
            }
    
            m_GeneralShaderData.TotalPointLights = m_TotalActivePointLights;
    
            for(int i = 0; i < m_TotalActivePointLights; i++)
            {
                auto& pointLight = m_PointLights[i];
                const Attenuation attenuation = pointLight->GetAttenuation();
    
                PointLightShaderData pointLightShaderData;
                pointLightShaderData.Position = pointLight->GetPosition();
                pointLightShaderData.Diffuse = pointLight->GetColor();
                pointLightShaderData.Intensity = pointLight->GetIntensity();
                pointLightShaderData.Constant = attenuation.Constant;
                pointLightShaderData.Linear = attenuation.Linear;
                pointLightShaderData.Quadratic = attenuation.Quadratic;
                pointLightShaderData.Specular = m_DefaultSpecularColor;
                pointLightShaderData.CastShadow = pointLight->IsCastShadowEnabled() ? 1 : 0;
    
                m_PointsShaderData[i] = pointLightShaderData;
            }
    
            m_GeneralShaderData.TotalSpotLights = m_TotalActiveSpotLights;
    
            for(int i = 0; i < m_TotalActiveSpotLights; i++)
            {
                auto& spotLight = m_SpotLights[i];
                const float cutoff = glm::cos(glm::radians(spotLight->GetInnerCutoffDegrees()));
                const float outerCutoff = glm::cos(glm::radians(spotLight->GetOuterCutoffDegrees()));
                const Attenuation attenuation = spotLight->GetAttenuation();
    
                SpotLightShaderData spotLightShaderData;
                spotLightShaderData.Position = spotLight->GetPosition();
                spotLightShaderData.Direction = spotLight->GetDirection();
                spotLightShaderData.Cutoff = cutoff;
                spotLightShaderData.OuterCutoff = outerCutoff;
                spotLightShaderData.Diffuse = spotLight->GetColor();
                spotLightShaderData.Intensity = spotLight->GetIntensity();
                spotLightShaderData.Constant = attenuation.Constant;
                spotLightShaderData.Linear = attenuation.Linear;
                spotLightShaderData.Quadratic = attenuation.Quadratic;
                spotLightShaderData.Specular = m_DefaultSpecularColor;
                spotLightShaderData.CastShadow = spotLight->IsCastShadowEnabled() ? 1 : 0;
    
                m_SpotsShaderData[i] = spotLightShaderData;
            }

            m_GeneralUniformBuffer->Bind();
            m_GeneralUniformBuffer->SetSubData(&m_GeneralShaderData, sizeof(LightingGeneralShaderData));
            m_GeneralUniformBuffer->Unbind();

            m_DirectionalUniformBuffer->Bind();
            m_DirectionalUniformBuffer->SetSubData(m_DirectionalsShaderData, MAX_DIRECTIONAL_LIGHTS * sizeof(DirectionalLightShaderData));
            m_DirectionalUniformBuffer->Unbind();

            m_PointUniformBuffer->Bind();
            m_PointUniformBuffer->SetSubData(m_PointsShaderData, MAX_POINT_LIGHTS * sizeof(PointLightShaderData));
            m_PointUniformBuffer->Unbind();

            m_SpotsUniformBuffer->Bind();
            m_SpotsUniformBuffer->SetSubData(m_SpotsShaderData, MAX_SPOT_LIGHTS * sizeof(SpotLightShaderData));
            m_SpotsUniformBuffer->Unbind();

            UpdateDirectionalShadowMapUniformBuffers();
            UpdatePointShadowMapUniformBuffers();
            UpdateSpotShadowMapUniformBuffers();

            // Binding shadow map texture too early on Setup end ups getting unbound somewhere else internally
            // Also, when binding after creating a new shadow map (new light added) it makes first light to not proper have its texture bound
            // TODO: investigate above issue to not have to bind it every frame (or if it is expected and we should do this way)
            BindShadowMapTextures();
        }

        void LightingSystem::UpdateDirectionalShadowMapUniformBuffers()
        {
            if(m_TotalActiveDirectionalLights == 0)
            {
                return;
            }

            for(int i = 0; i < m_TotalActiveDirectionalLights; i++)
            {
                m_DirectionalLightShadowMapShaderData[i].ViewProjectionMatrix = m_DirectionalLights[i]->GetViewProjectionMatrix();
            }

            m_DirectionalMatrixUniformBuffer->Bind();
            m_DirectionalMatrixUniformBuffer->SetSubData(m_DirectionalLightShadowMapShaderData, MAX_DIRECTIONAL_LIGHTS * sizeof(glm::mat4));
            m_DirectionalMatrixUniformBuffer->Unbind();
        }

        void LightingSystem::UpdatePointShadowMapUniformBuffers()
        {
            if(m_TotalActivePointLights == 0)
            {
                return;
            }
    
            for(int i = 0; i < m_TotalActivePointLights; i++)
            {
                std::vector<glm::mat4> lightSpaceMatrices = m_PointLights[i]->GetViewProjectionMatrices(m_ShadowResolution);

                // TODO: maybe we could use pointers to avoid another loop
                for(int j = 0; j < static_cast<int>(lightSpaceMatrices.size()); j++)
                {
                    PointLightShadowMapShaderData& shaderData = m_PointLightShadowMapShaderData[i];
                    shaderData.ViewProjectionMatrices[j] = lightSpaceMatrices[j];
                }
            }

            m_PointLightMatricesUniformBuffer->Bind();
            m_PointLightMatricesUniformBuffer->SetSubData(m_PointLightShadowMapShaderData, MAX_POINT_LIGHTS * sizeof(PointLightShadowMapShaderData));
            m_PointLightMatricesUniformBuffer->Unbind();
        }

        void LightingSystem::UpdateSpotShadowMapUniformBuffers()
        {
            if(m_TotalActiveSpotLights == 0)
            {
                return;
            }

            for(int i = 0; i < m_TotalActiveSpotLights; i++)
            {
                m_SpotLightShadowMapShaderData[i].ViewProjectionMatrix = m_SpotLights[i]->GetViewProjectionMatrix(m_ShadowResolution);
            }

            m_SpotLightMatricesUniformBuffer->Bind();
            m_SpotLightMatricesUniformBuffer->SetSubData(m_SpotLightShadowMapShaderData, MAX_SPOT_LIGHTS * sizeof(SpotLightShadowMapShaderData));
            m_SpotLightMatricesUniformBuffer->Unbind();
        }

        Framebuffer& LightingSystem::GetDirectionalShadowMapFramebuffer(const int activeLightIndex) const
        {
            ASSERT(activeLightIndex >= 0 && activeLightIndex < m_TotalActiveDirectionalLights);
            return *m_DirectionalShadowMapBuffers[activeLightIndex];
        }

        const DirectionalLightComponent& LightingSystem::GetDirectionalLight(const int activeLightIndex) const
        {
            ASSERT(activeLightIndex >= 0 && activeLightIndex < m_TotalActiveDirectionalLights);
            return *m_DirectionalLights[activeLightIndex];
        }

        const Framebuffer& LightingSystem::GetPointLightShadowMapFramebuffer(const int activeLightIndex) const
        {
            ASSERT(activeLightIndex >= 0 && activeLightIndex < m_TotalActivePointLights);
            return *m_PointLightShadowMapBuffers[activeLightIndex];
        }

        const PointLightComponent& LightingSystem::GetPointLight(const int activeLightIndex) const
        {
            ASSERT(activeLightIndex >= 0 && activeLightIndex < m_TotalActivePointLights);
            return *m_PointLights[activeLightIndex];
        }

        const Framebuffer& LightingSystem::GetSpotLightShadowMapFramebuffer(const int activeLightIndex) const
        {
            ASSERT(activeLightIndex >= 0 && activeLightIndex < m_TotalActiveSpotLights);
            return *m_SpotLightShadowMapBuffers[activeLightIndex];
        }

        const SpotLightComponent& LightingSystem::GetSpotLight(const int activeLightIndex) const
        {
            ASSERT(activeLightIndex >= 0 && activeLightIndex < m_TotalActiveSpotLights);
            return *m_SpotLights[activeLightIndex];
        }

        void LightingSystem::CreateUniformBuffers()
        {
            constexpr unsigned int UNIFORM_LIGHTING_GENERAL_BINDING_INDEX = 2;
            constexpr unsigned int UNIFORM_LIGHTING_DIRECTIONALS_BINDING_INDEX = 3;
            constexpr unsigned int UNIFORM_LIGHTING_POINTS_BINDING_INDEX = 4;
            constexpr unsigned int UNIFORM_LIGHTING_SPOTS_BINDING_INDEX = 5;
            constexpr unsigned int UNIFORM_LIGHTING_DIRECTIONAL_MATRIX_BINDING_INDEX = 6;
            constexpr unsigned int UNIFORM_LIGHTING_POINT_MATRIX_BINDING_INDEX = 7;
            constexpr unsigned int UNIFORM_LIGHTING_SPOT_MATRIX_BINDING_INDEX = 8;

            m_GeneralUniformBuffer = std::make_unique<UniformBuffer>(
                nullptr,
                sizeof(LightingGeneralShaderData),
                UNIFORM_LIGHTING_GENERAL_BINDING_INDEX,
                "LightingGeneral",
                true);

            m_DirectionalUniformBuffer = std::make_unique<UniformBuffer>(
                nullptr,
                MAX_DIRECTIONAL_LIGHTS * sizeof(DirectionalLightShaderData),
                UNIFORM_LIGHTING_DIRECTIONALS_BINDING_INDEX,
                "LightingDirectionals",
                true);

            m_PointUniformBuffer = std::make_unique<UniformBuffer>(
                nullptr,
                MAX_POINT_LIGHTS * sizeof(PointLightShaderData),
                UNIFORM_LIGHTING_POINTS_BINDING_INDEX,
                "LightingPoints",
                true);

            m_SpotsUniformBuffer = std::make_unique<UniformBuffer>(
                nullptr,
                MAX_SPOT_LIGHTS * sizeof(SpotLightShaderData),
                UNIFORM_LIGHTING_SPOTS_BINDING_INDEX,
                "LightingSpots",
                true);

            m_DirectionalMatrixUniformBuffer = std::make_unique<UniformBuffer>(
                nullptr,
                MAX_DIRECTIONAL_LIGHTS * sizeof(glm::mat4),
                UNIFORM_LIGHTING_DIRECTIONAL_MATRIX_BINDING_INDEX,
                "DirectionalLightShadowMapMatrices",
                true);

            m_PointLightMatricesUniformBuffer = std::make_unique<UniformBuffer>(
                nullptr,
                MAX_POINT_LIGHTS * sizeof(PointLightShadowMapShaderData),
                UNIFORM_LIGHTING_POINT_MATRIX_BINDING_INDEX,
                "PointLightShadowMapMatrices",
                true);

            m_SpotLightMatricesUniformBuffer = std::make_unique<UniformBuffer>(
                nullptr,
                MAX_SPOT_LIGHTS * sizeof(SpotLightShadowMapShaderData),
                UNIFORM_LIGHTING_SPOT_MATRIX_BINDING_INDEX,
                "SpotLightShadowMapMatrices",
                true);
        }

        void LightingSystem::BindShadowMapTextures()
        {
            for(int i = 0; i < m_TotalActiveDirectionalLights; i++)
            {
                std::shared_ptr<Texture> directionalDepthTexture = m_DirectionalShadowMapBuffers[i]->GetDepthBufferTexture();
                directionalDepthTexture->Bind(DIRECTIONAL_SHADOW_MAP_START_SLOT + i);
            }

            for(int i = 0; i < m_TotalActivePointLights; i++)
            {
                std::shared_ptr<Cubemap> pointDepthCubemap = m_PointLightShadowMapBuffers[i]->GetDepthBufferCubemap();
                pointDepthCubemap->Bind(POINT_SHADOW_MAP_START_SLOT + i);
            }

            for(int i = 0; i < m_TotalActiveSpotLights; i++)
            {
                std::shared_ptr<Texture> spotDepthTexture = m_SpotLightShadowMapBuffers[i]->GetDepthBufferTexture();
                spotDepthTexture->Bind(SPOT_SHADOW_MAP_START_SLOT + i);
            }
        }

        void LightingSystem::UnbindShadowMapTextures()
        {
            for(int i = 0; i < m_TotalActiveDirectionalLights; i++)
            {
                std::shared_ptr<Texture> directionalDepthTexture = m_DirectionalShadowMapBuffers[i]->GetDepthBufferTexture();
                directionalDepthTexture->Unbind(DIRECTIONAL_SHADOW_MAP_START_SLOT + i);
            }

            for(int i = 0; i < m_TotalActivePointLights; i++)
            {
                std::shared_ptr<Cubemap> pointDepthCubemap = m_PointLightShadowMapBuffers[i]->GetDepthBufferCubemap();
                pointDepthCubemap->Unbind(POINT_SHADOW_MAP_START_SLOT + i);
            }

            for(int i = 0; i < m_TotalActiveSpotLights; i++)
            {
                std::shared_ptr<Texture> spotDepthTexture = m_SpotLightShadowMapBuffers[i]->GetDepthBufferTexture();
                spotDepthTexture->Unbind(SPOT_SHADOW_MAP_START_SLOT + i);
            }
        }

        void LightingSystem::CreateShadowMaps()
        {
            m_DirectionalShadowMapBuffers.reserve(MAX_DIRECTIONAL_LIGHTS);
            m_PointLightShadowMapBuffers.reserve(MAX_POINT_LIGHTS);
            m_SpotLightShadowMapBuffers.reserve(MAX_SPOT_LIGHTS);
        }

        void LightingSystem::CreateDirectionalLightShadowMapFor(int lightIndex)
        {
            ASSERT(lightIndex >= 0 && lightIndex < static_cast<int>(m_DirectionalShadowMapBuffers.capacity()));

            FramebufferSettings settings{};
            settings.Resolution = m_DirectionalShadowResolution;
            settings.EnableDepthMapOnly = true;
    
            m_DirectionalShadowMapBuffers.emplace_back(std::make_unique<Framebuffer>(settings));
        }

        void LightingSystem::CreatePointLightShadowMapFor(int lightIndex)
        {
            assert(lightIndex >= 0 && lightIndex < static_cast<int>(m_PointLightShadowMapBuffers.capacity()));

            FramebufferSettings settings{};
            settings.Resolution = m_ShadowResolution;
            settings.EnableDepthMapOnly = true;
            settings.UseDepthCubemap = true;
    
            m_PointLightShadowMapBuffers.emplace_back(std::make_unique<Framebuffer>(settings));
        }

        void LightingSystem::CreateSportLightShadowMapFor(int lightIndex)
        {
            ASSERT(lightIndex >= 0 && lightIndex < static_cast<int>(m_SpotLightShadowMapBuffers.capacity()));

            FramebufferSettings settings{};
            settings.Resolution = m_ShadowResolution;
            settings.EnableDepthMapOnly = true;
    
            m_SpotLightShadowMapBuffers.emplace_back(std::make_unique<Framebuffer>(settings));
        }
    }
}
