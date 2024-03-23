#pragma once
#include <memory>

class Mesh;

class Primitive
{
public:

    static std::shared_ptr<Mesh> CreateCube();
};
