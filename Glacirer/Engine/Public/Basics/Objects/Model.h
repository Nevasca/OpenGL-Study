#pragma once
#include "GameObject/GameObject.h"

namespace Glacirer
{
    namespace Rendering
    {
        class ModelData;
        class Material;
    }

    class ENGINE_API Model : public GameObject
    {
        GENERATE_OBJECT_BODY(Model)
    
    public:

        void Setup(const std::shared_ptr<Rendering::ModelData>& modelData, const std::shared_ptr<Rendering::Material>& material);
    };
}
