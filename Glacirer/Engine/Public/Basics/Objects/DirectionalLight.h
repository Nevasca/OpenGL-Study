#pragma once
#include "GameObject/GameObject.h"
#include "Basics/Components/DirectionalLightComponent.h"

namespace Glacirer
{
    class DirectionalLightComponent;

    class ENGINE_API DirectionalLight : public GameObject
    {
        GENERATE_OBJECT_BODY(DirectionalLight)
    
    public:

        void Initialize() override;
        void Destroy() override;

        void SetColor(const glm::vec3& color);
        void SetIntensity(const float intensity);
        void SetCastShadowEnabled(const bool enable);

    private:

        std::weak_ptr<DirectionalLightComponent> m_LightComponent{};
    };
}
