#pragma once
#include <memory>

class Mesh;

class MeshResource
{
public:

    static std::shared_ptr<Mesh> LoadCube();
};
