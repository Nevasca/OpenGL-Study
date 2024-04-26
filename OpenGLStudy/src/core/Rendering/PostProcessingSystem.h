#pragma once
#include <memory>

#include "MeshRenderer.h"

class PostProcessingComponent;
class Mesh;
class Framebuffer;

class PostProcessingSystem
{
public:

    PostProcessingSystem();
    
    void SetFramebuffer(const Framebuffer& framebuffer) const;
    void SetPostProcessingComponent(const std::shared_ptr<PostProcessingComponent>& postProcessingComponent);
    void RemovePostProcessingComponent(const std::shared_ptr<PostProcessingComponent>& postProcessingComponent);
    void RenderToScreen();

private:

    std::shared_ptr<Mesh> m_ScreenQuad{};
    MeshRenderer m_MeshRenderer{};
    std::shared_ptr<Material> m_PostProcessingMaterial{};
    std::shared_ptr<PostProcessingComponent> m_PostProcessingComponent{};

    void UpdatePostProcessingMaterial();
};
