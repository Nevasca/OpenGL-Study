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
        ResourceManager::RESOURCES_PATH + "Textures/skybox/right.jpg",
        ResourceManager::RESOURCES_PATH + "Textures/skybox/left.jpg",
        ResourceManager::RESOURCES_PATH + "Textures/skybox/top.jpg",
        ResourceManager::RESOURCES_PATH + "Textures/skybox/bottom.jpg",
        ResourceManager::RESOURCES_PATH + "Textures/skybox/back.jpg",
        ResourceManager::RESOURCES_PATH + "Textures/skybox/front.jpg"
    };
    
    std::shared_ptr<Rendering::Cubemap> skyCubemap = ResourceManager::GetOrLoadCubemap(skyCubemapSettings, "C_Sky");

    const std::string SKYBOX_SHADER_NAME = "S_Skybox";
    std::shared_ptr<Shader> skyboxShader = ResourceManager::GetOrLoadShader(ResourceManager::RESOURCES_PATH + "Shaders/Skybox.glsl", SKYBOX_SHADER_NAME);
    std::shared_ptr<Material> skyMaterial = ResourceManager::GetOrCreateMaterial("M_Skybox", SKYBOX_SHADER_NAME);

    m_SkyboxComponent->SetCubemap(skyCubemap);
    m_SkyboxComponent->SetMaterial(skyMaterial);
}
