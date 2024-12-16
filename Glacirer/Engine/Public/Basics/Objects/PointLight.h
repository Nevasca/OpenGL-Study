#pragma once
#include "GameObject/GameObject.h"

#include "Basics/Components/PointLightComponent.h"

namespace Glacirer
{
    class ENGINE_API PointLight : public GameObject
    {
        GENERATE_OBJECT_BODY(PointLight)
    
    public:

        void Initialize() override;

        void SetColor(const glm::vec3& color);
        void SetRange(float range);
        void SetIntensity(const float intensity);
        void SetCastShadowEnabled(const bool enable);

    private:

        std::weak_ptr<PointLightComponent> m_LightComponent{};
    };
}
