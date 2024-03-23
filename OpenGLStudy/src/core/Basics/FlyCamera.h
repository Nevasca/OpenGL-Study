#pragma once
#include "core/GameObject/GameObject.h"

class FlyCamera : public GameObject
{
    GENERATE_OBJECT_BODY(FlyCamera)
    
public:

    void Initialize() override;
};
