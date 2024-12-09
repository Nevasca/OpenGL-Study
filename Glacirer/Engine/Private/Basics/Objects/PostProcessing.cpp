#include "Basics/Objects/PostProcessing.h"

namespace Glacirer
{
    void PostProcessing::Initialize()
    {
        GameObject::Initialize();

        m_PostProcessingComponent = AddComponent<PostProcessingComponent>();
    }
}
