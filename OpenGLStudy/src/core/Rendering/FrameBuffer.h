#pragma once
#include <memory>
#include <vector>

#include "Resolution.h"
#include "Texture.h"
#include "glm/vec4.hpp"

struct FramebufferSettings
{
    Rendering::Resolution Resolution{};
    bool EnableDepthMapOnly{false};
    bool EnableDepthBuffer{true};
    bool EnableDepthBufferWriteOnly{true};
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
    std::shared_ptr<Texture> GetDepthBufferTexture() const { return m_DepthBufferTexture; }
    Rendering::Resolution GetResolution() const { return m_Resolution; }

    void SetClearColor(const glm::vec4& clearColor) { m_ClearColor = clearColor; }
    glm::vec4 GetClearColor() const { return m_ClearColor; }

    void ResolveMultisampleImage(const Rendering::Resolution& destinationResolution) const;

private:
    
    unsigned int m_FBO{0};
    unsigned int m_RBO{0};
    std::shared_ptr<Texture> m_MainColorBufferTexture{};
    std::shared_ptr<Texture> m_DepthBufferTexture{};
    std::vector<std::shared_ptr<Texture>> m_AdditionalColorTextures{};
    glm::vec4 m_ClearColor{0.f, 0.f, 0.f, 1.f};
    Rendering::Resolution m_Resolution{};

    void Create(const FramebufferSettings& settings);
    void CreateColorAttachments(const FramebufferSettings& settings);
    void CreateAdditionalColorAttachments(const std::vector<TextureSettings>& additionalColorAttachments);
    void EnableAdditionalColorAttachments(unsigned int totalAdditionalColorAttachments);
    void CreateDepthMapAttachment(const FramebufferSettings& settings);
    void CreateRenderBuffer(const unsigned int samples);
};
