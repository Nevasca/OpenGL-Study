#pragma once
#include <memory>
#include <vector>

#include "Texture.h"
#include "glm/vec4.hpp"

class Framebuffer
{
public:
    Framebuffer(unsigned int width, unsigned int height, const std::vector<TextureSettings>& additionalColorAttachments);
    ~Framebuffer();

    void Bind() const;
    void BindAndClear() const;
    void Unbind() const;
    std::shared_ptr<Texture> GetAdditionalColorTexture(unsigned int index) const;

    std::shared_ptr<Texture> GetMainColorBufferTexture() const { return m_MainColorBufferTexture; }
    void SetClearColor(const glm::vec4& clearColor) { m_ClearColor = clearColor; }
    glm::vec4 GetClearColor() const { return m_ClearColor; }

private:
    
    unsigned int m_FBO;
    std::shared_ptr<Texture> m_MainColorBufferTexture{};
    std::vector<std::shared_ptr<Texture>> m_AdditionalColorTextures{};
    glm::vec4 m_ClearColor{0.f, 0.f, 0.f, 1.f};

    void CreateAdditionalColorAttachments(unsigned int width, unsigned int height, const std::vector<TextureSettings>& additionalColorAttachments);
    void EnableAdditionalColorAttachments(unsigned int totalAdditionalColorAttachments);
};
