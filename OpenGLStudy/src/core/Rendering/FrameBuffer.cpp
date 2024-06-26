#include "FrameBuffer.h"

#include <iostream>

#include "OpenGLCore.h"

Framebuffer::Framebuffer(unsigned int width, unsigned int height, bool bIsDepthTestEnabled, const std::vector<TextureSettings>& additionalColorAttachments)
{
    GLCall(glGenFramebuffers(1, &m_FBO));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));

    TextureSettings mainColorTextureSettings{};
    mainColorTextureSettings.InternalFormat = GL_RGB;
    mainColorTextureSettings.Format = GL_RGB;

    m_MainColorBufferTexture = std::make_shared<Texture>(nullptr, width, height, mainColorTextureSettings);

    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_MainColorBufferTexture->GetRendererID(), 0));

    CreateAdditionalColorAttachments(width, height, additionalColorAttachments);
    EnableAdditionalColorAttachments(static_cast<unsigned int>(additionalColorAttachments.size()));

    // TODO: allow a setting for using a depth texture attachment instead of using RenderBuffer 
    // if we intend on using it for some effect
    if(bIsDepthTestEnabled)
    {
        CreateRenderBuffer(width, height);
    }
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR:FRAMEBUFFER: Framebuffer is not complete!\n";
    }

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void Framebuffer::CreateAdditionalColorAttachments(unsigned int width, unsigned int height, const std::vector<TextureSettings>& additionalColorAttachments)
{
    if(additionalColorAttachments.empty())
    {
        return;        
    }

    m_AdditionalColorTextures.reserve(additionalColorAttachments.size());

    for(unsigned int i = 0; i < additionalColorAttachments.size(); i++)
    {
        std::shared_ptr<Texture> additionalColorTexture = std::make_shared<Texture>(nullptr, width, height, additionalColorAttachments[i]);

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

void Framebuffer::CreateRenderBuffer(unsigned int width, unsigned int height)
{
    // Render buffer for depth and stencil test
    // Render buffer does not allow easy access, if we intend on using depth buffer for some effect
    // we need to create a depth buffer attachment
    GLCall(glGenRenderbuffers(1, &m_RBO));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_RBO));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
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
}

void Framebuffer::Bind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
}

void Framebuffer::BindAndClear() const
{
    Bind();

    GLCall(glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void Framebuffer::Unbind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
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
