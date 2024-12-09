#pragma once
#include <memory>
#include <vector>

#include "Cubemap.h"
#include "Resolution.h"
#include "Texture.h"
#include <glm/vec4.hpp>

namespace Glacirer
{
    namespace Rendering
    {
        struct FramebufferSettings
        {
            Rendering::Resolution Resolution{};
            bool EnableDepthMapOnly{false};
            bool EnableDepthBuffer{true};
            bool EnableDepthBufferWriteOnly{true};
            bool UseDepthCubemap{false};
            std::vector<TextureSettings> AdditionalColorAttachments{};
            unsigned int Samples{1};
        };

        class Framebuffer
        {
        public:
            Framebuffer(
                const Rendering::Resolution& resolution,
                bool bIsDepthTestEnabled,
                const std::vector<TextureSettings>& additionalColorAttachments,
                const unsigned int samples = 1);

            Framebuffer(const FramebufferSettings& settings);

            ~Framebuffer();

            void Bind() const;
            void BindAndClear() const;
            void BindAsReadOnly() const;
            void BindAsWriteOnly() const;
            void Unbind() const;
            void Clear() const;

            std::shared_ptr<Texture> GetAdditionalColorTexture(unsigned int index) const;
            std::shared_ptr<Texture> GetMainColorBufferTexture() const { return m_MainColorBufferTexture; }
            // TODO: instead of having two different depth getters, we could add a interface such as ISamplable
            std::shared_ptr<Texture> GetDepthBufferTexture() const { return m_DepthBufferTexture; }
            std::shared_ptr<Rendering::Cubemap> GetDepthBufferCubemap() const { return m_DepthBufferCubemap; }
            Rendering::Resolution GetResolution() const { return m_Resolution; }

            void SetClearColor(const glm::vec4& clearColor) { m_ClearColor = clearColor; }
            glm::vec4 GetClearColor() const { return m_ClearColor; }

            void ResolveMultisampleImage(const Rendering::Resolution& destinationResolution) const;

        private:
    
            unsigned int m_FBO{0};
            unsigned int m_RBO{0};
            std::shared_ptr<Texture> m_MainColorBufferTexture{};
            std::shared_ptr<Texture> m_DepthBufferTexture{};
            std::shared_ptr<Rendering::Cubemap> m_DepthBufferCubemap{};
            std::vector<std::shared_ptr<Texture>> m_AdditionalColorTextures{};
            glm::vec4 m_ClearColor{0.f, 0.f, 0.f, 1.f};
            Rendering::Resolution m_Resolution{};

            void Create(const FramebufferSettings& settings);
            void CreateColorAttachments(const FramebufferSettings& settings);
            void CreateAdditionalColorAttachments(const std::vector<TextureSettings>& additionalColorAttachments);
            void EnableAdditionalColorAttachments(unsigned int totalAdditionalColorAttachments);
            void CreateDepthMap2DAttachment(const FramebufferSettings& settings);
            void CreateDepthMapCubemapAttachment(const FramebufferSettings& settings);
            void CreateRenderBuffer(const unsigned int samples);
        };
    }
}
