#pragma once
#include "core/GameObject/GameObject.h"

class FlyingCamera : public GameObject
{
    GENERATE_OBJECT_BODY(FlyingCamera)
    
public:

    void Initialize() override;
};
