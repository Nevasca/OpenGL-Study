#pragma once
#include "core/GameObject/GameObject.h"

class ModelData;
class Material;

class Model : public GameObject
{
    GENERATE_OBJECT_BODY(Model)
    
public:

    void Setup(const std::shared_ptr<ModelData>& modelData, const std::shared_ptr<Material>& material);
};
