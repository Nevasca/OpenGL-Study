#include "MeshResource.h"

#include "core/Rendering/Primitive.h"

std::shared_ptr<Mesh> MeshResource::LoadCube()
{
    return Primitive::CreateCube();
}
