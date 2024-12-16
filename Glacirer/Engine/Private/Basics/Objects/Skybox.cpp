#include "Basics/Objects/Skybox.h"

#include "Resources/ResourceManager.h"
#include "Basics/Components/SkyboxComponent.h"
#include "Rendering/Cubemap.h"

namespace Glacirer
{
    void Skybox::Initialize()
    {
        GameObject::Initialize();

        m_SkyboxComponent = AddComponent<SkyboxComponent>();
    }

    void Skybox::SetDefaultSky()
    {
        std::shared_ptr<SkyboxComponent> skyboxComponent = m_SkyboxComponent.lock();

        if(!skyboxComponent)
        {
            return;
        }
        
        Rendering::CubemapLoadSettings skyCubemapSettings{
            Resources::ResourceManager::RESOURCES_PATH + "Textures/skybox/right.jpg",
            Resources::ResourceManager::RESOURCES_PATH + "Textures/skybox/left.jpg",
            Resources::ResourceManager::RESOURCES_PATH + "Textures/skybox/top.jpg",
            Resources::ResourceManager::RESOURCES_PATH + "Textures/skybox/bottom.jpg",
            Resources::ResourceManager::RESOURCES_PATH + "Textures/skybox/front.jpg",
            Resources::ResourceManager::RESOURCES_PATH + "Textures/skybox/back.jpg"
        };                             
    
        std::shared_ptr<Rendering::Cubemap> skyCubemap = Resources::ResourceManager::GetOrLoadCubemap(skyCubemapSettings, "C_Sky");

        const std::string SKYBOX_SHADER_NAME = "S_Skybox";
        std::shared_ptr<Rendering::Shader> skyboxShader = Resources::ResourceManager::GetOrLoadShader(Resources::ResourceManager::RESOURCES_PATH + "Shaders/Skybox.glsl", SKYBOX_SHADER_NAME);
        std::shared_ptr<Rendering::Material> skyMaterial = Resources::ResourceManager::GetOrCreateMaterial("M_Skybox", SKYBOX_SHADER_NAME);

        skyboxComponent->SetCubemap(skyCubemap);
        skyboxComponent->SetMaterial(skyMaterial);
    }
}
