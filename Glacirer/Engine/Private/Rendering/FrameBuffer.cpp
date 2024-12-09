#include "Rendering/FrameBuffer.h"

#include <iostream>

#include "Rendering/OpenGLCore.h"

namespace Glacirer
{
    namespace Rendering
    {
        Framebuffer::Framebuffer(
            const Rendering::Resolution& resolution,
            bool bIsDepthTestEnabled,
            const std::vector<TextureSettings>& additionalColorAttachments,
            const unsigned int samples)
            : m_Resolution(resolution)
        {
            FramebufferSettings settings{};
            settings.Resolution = resolution;
            settings.EnableDepthBuffer = bIsDepthTestEnabled;
            settings.AdditionalColorAttachments = additionalColorAttachments;
            settings.Samples = samples;

            Create(settings);
        }

        Framebuffer::Framebuffer(const FramebufferSettings& settings)
            : m_Resolution(settings.Resolution)
        {
            Create(settings);
        }

        void Framebuffer::Create(const FramebufferSettings& settings)
        {
            GLCall(glGenFramebuffers(1, &m_FBO));
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));

            if(!settings.EnableDepthMapOnly)
            {
                CreateColorAttachments(settings);
            }

            if(settings.EnableDepthMapOnly || !settings.EnableDepthBufferWriteOnly)
            {
                if(settings.UseDepthCubemap)
                {
                    CreateDepthMapCubemapAttachment(settings);
                }
                else
                {
                    CreateDepthMap2DAttachment(settings);
                }
            }
            else if(settings.EnableDepthBuffer || settings.Samples > 1)
            {
                CreateRenderBuffer(settings.Samples);
            }

            bool bSuccess = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE; 
            assert(bSuccess);

            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        }

        void Framebuffer::CreateColorAttachments(const FramebufferSettings& settings)
        {
            TextureSettings mainColorTextureSettings{};
            mainColorTextureSettings.InternalFormat = GL_RGB;
            mainColorTextureSettings.Format = GL_RGB;
            mainColorTextureSettings.Samples = settings.Samples;

            m_MainColorBufferTexture = std::make_shared<Texture>(nullptr, settings.Resolution.Width, settings.Resolution.Height, mainColorTextureSettings);

            unsigned int targetColorTexture = settings.Samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, targetColorTexture, m_MainColorBufferTexture->GetRendererID(), 0));

            CreateAdditionalColorAttachments(settings.AdditionalColorAttachments);
            EnableAdditionalColorAttachments(static_cast<unsigned int>(settings.AdditionalColorAttachments.size()));
        }

        void Framebuffer::CreateAdditionalColorAttachments(const std::vector<TextureSettings>& additionalColorAttachments)
        {
            if(additionalColorAttachments.empty())
            {
                return;        
            }

            m_AdditionalColorTextures.reserve(additionalColorAttachments.size());

            for(unsigned int i = 0; i < additionalColorAttachments.size(); i++)
            {
                std::shared_ptr<Texture> additionalColorTexture = std::make_shared<Texture>(nullptr, m_Resolution.Width, m_Resolution.Height, additionalColorAttachments[i]);

                GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1 + i, GL_TEXTURE_2D, additionalColorTexture->GetRendererID(), 0));
                m_AdditionalColorTextures.emplace_back(std::move(additionalColorTexture));
            }
        }

        void Framebuffer::EnableAdditionalColorAttachments(unsigned int totalAdditionalColorAttachments)
        {
            std::vector<GLenum> drawBuffers{};
            drawBuffers.reserve(1 + totalAdditionalColorAttachments);
            drawBuffers.push_back(GL_COLOR_ATTACHMENT0);

            for(unsigned int i = 0; i < totalAdditionalColorAttachments; i++)
            {
                drawBuffers.push_back(GL_COLOR_ATTACHMENT1 + i);
            }
    
            GLCall(glDrawBuffers(1 + totalAdditionalColorAttachments, drawBuffers.data()));
        }

        void Framebuffer::CreateDepthMap2DAttachment(const FramebufferSettings& settings)
        {
            TextureSettings depthTextureSettings{};
            depthTextureSettings.InternalFormat = GL_DEPTH_COMPONENT;
            depthTextureSettings.Format = GL_DEPTH_COMPONENT;
            depthTextureSettings.Type = GL_FLOAT;
            // TODO: tutorial uses GL_NEAREST, but it's reproducing worse results, investigate how to have better shadows
            // depthTextureSettings.MinFilter = GL_NEAREST;
            // depthTextureSettings.MagFilter = GL_NEAREST;
            depthTextureSettings.Samples = settings.Samples;

            // When using a depth texture for shadow mapping, to partially fix oversampling
            // we clamp texture to a white border so when sampling outside light frustum it returns not in shadow
            depthTextureSettings.UseBorder = true;
            depthTextureSettings.WrapS = GL_CLAMP_TO_BORDER;
            depthTextureSettings.WrapT = GL_CLAMP_TO_BORDER;
            depthTextureSettings.BorderColor = glm::vec4{1.f};

            m_DepthBufferTexture = std::make_shared<Texture>(nullptr, settings.Resolution.Width, settings.Resolution.Height, depthTextureSettings);

            unsigned int targetDepthTexture = settings.Samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, targetDepthTexture, m_DepthBufferTexture->GetRendererID(), 0));

            if(settings.EnableDepthMapOnly)
            {
                GLCall(glDrawBuffer(GL_NONE));
                GLCall(glReadBuffer(GL_NONE));
            }
        }

        void Framebuffer::CreateDepthMapCubemapAttachment(const FramebufferSettings& settings)
        {
            TextureSettings depthCubemapSettings{};
            depthCubemapSettings.InternalFormat = GL_DEPTH_COMPONENT;
            depthCubemapSettings.Format = GL_DEPTH_COMPONENT;
            depthCubemapSettings.Type = GL_FLOAT;
            depthCubemapSettings.Samples = settings.Samples;
            depthCubemapSettings.MinFilter = GL_NEAREST;
            depthCubemapSettings.MagFilter = GL_NEAREST;

            m_DepthBufferCubemap = std::make_shared<Rendering::Cubemap>(settings.Resolution.Width, settings.Resolution.Height, depthCubemapSettings);

            GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthBufferCubemap->GetRendererId(), 0));

            if(settings.EnableDepthMapOnly)
            {
                GLCall(glDrawBuffer(GL_NONE));
                GLCall(glReadBuffer(GL_NONE));
            }
        }

        void Framebuffer::CreateRenderBuffer(const unsigned int samples)
        {
            // Render buffer for depth and stencil test
            // Render buffer does not allow easy access, if we intend on using depth buffer for some effect
            // we need to create a depth buffer attachment
            GLCall(glGenRenderbuffers(1, &m_RBO));
            GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RBO));

            if(samples > 1)
            {
                GLCall(glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, m_Resolution.Width, m_Resolution.Height));        
            }
            else
            {
                GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Resolution.Width, m_Resolution.Height));
            }

            GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));

            GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO));
        }

        Framebuffer::~Framebuffer()
        {
            Unbind();

            GLCall(glDeleteFramebuffers(1, &m_FBO));

            if(m_RBO > 0)
            {
                GLCall(glDeleteRenderbuffers(1, &m_RBO));
            }

            m_MainColorBufferTexture.reset();
            m_AdditionalColorTextures.clear();
            m_DepthBufferTexture.reset();
            m_DepthBufferCubemap.reset();
        }

        void Framebuffer::Bind() const
        {
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
        }

        void Framebuffer::BindAndClear() const
        {
            Bind();
            Clear();
        }

        void Framebuffer::BindAsReadOnly() const
        {
            GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO));
        }

        void Framebuffer::BindAsWriteOnly() const
        {
            GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO));
        }

        void Framebuffer::Unbind() const
        {
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        }

        void Framebuffer::Clear() const
        {
            GLCall(glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a));
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
        }

        std::shared_ptr<Texture> Framebuffer::GetAdditionalColorTexture(unsigned int index) const
        {
            if(index >= static_cast<unsigned int>(m_AdditionalColorTextures.size()))
            {
                std::cout << "ERROR:FRAMEBUFFER Trying to get an invalid additional color texture with index " << index << "\n";
                return nullptr;
            }

            return m_AdditionalColorTextures[index];
        }

        void Framebuffer::ResolveMultisampleImage(const Rendering::Resolution& destinationResolution) const
        {
            glBlitFramebuffer(
                0,
                0,
                static_cast<int>(m_Resolution.Width),
                static_cast<int>(m_Resolution.Height),
                0,
                0,
                static_cast<int>(destinationResolution.Width),
                static_cast<int>(destinationResolution.Height),
                GL_COLOR_BUFFER_BIT,
                GL_NEAREST);
        }
    }
}
