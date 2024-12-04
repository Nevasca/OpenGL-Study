#include "Basics/Objects/Skybox.h"

#include "ResourceManager.h"
#include "Basics/Components/SkyboxComponent.h"
#include "Rendering/Cubemap.h"

void Skybox::Initialize()
{
    GameObject::Initialize();

    m_SkyboxComponent = AddComponent<SkyboxComponent>();
}

void Skybox::SetDefaultSky()
{
    Rendering::CubemapLoadSettings skyCubemapSettings{
        "res/textures/skybox/right.jpg",
        "res/textures/skybox/left.jpg",
        "res/textures/skybox/top.jpg",
        "res/textures/skybox/bottom.jpg",
        "res/textures/skybox/back.jpg",
        "res/textures/skybox/front.jpg"
    };
    
    std::shared_ptr<Rendering::Cubemap> skyCubemap = ResourceManager::GetOrLoadCubemap(skyCubemapSettings, "C_Sky");

    const std::string SKYBOX_SHADER_NAME = "S_Skybox";
    std::shared_ptr<Shader> skyboxShader = ResourceManager::GetOrLoadShader("res/core/shaders/Skybox.glsl", SKYBOX_SHADER_NAME);
    std::shared_ptr<Material> skyMaterial = ResourceManager::GetOrCreateMaterial("M_Skybox", SKYBOX_SHADER_NAME);

    m_SkyboxComponent->SetCubemap(skyCubemap);
    m_SkyboxComponent->SetMaterial(skyMaterial);
}
