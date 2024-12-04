#pragma once
#include "GameObject/GameObject.h"

class SkyboxComponent;

class Skybox : public GameObject
{
    GENERATE_OBJECT_BODY(Skybox)

public:

    void Initialize() override;

    void SetDefaultSky();

private:

    std::shared_ptr<SkyboxComponent> m_SkyboxComponent{};
};
