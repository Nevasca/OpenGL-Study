#include "PostProcessing.h"

void PostProcessing::Initialize()
{
    GameObject::Initialize();

    m_PostProcessingComponent = AddComponent<PostProcessingComponent>();
}
