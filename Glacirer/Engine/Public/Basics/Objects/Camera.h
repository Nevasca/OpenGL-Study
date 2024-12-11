#pragma once

#include "GameObject/GameObject.h"

namespace Glacirer
{
    class CameraComponent;
}

namespace Glacirer
{
    class ENGINE_API Camera : public GameObject
    {
        GENERATE_OBJECT_BODY(Camera)

    public:
        void Initialize() override;

    private:
        std::shared_ptr<CameraComponent> m_CameraComponent; 
    };
}
