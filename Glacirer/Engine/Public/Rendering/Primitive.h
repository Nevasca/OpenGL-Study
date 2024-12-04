#pragma once
#include <memory>

class Mesh;

class Primitive
{
public:

    static std::shared_ptr<Mesh> CreateCube();
    static std::shared_ptr<Mesh> CreateQuad();
    static std::shared_ptr<Mesh> CreateScreenQuad();
    static std::shared_ptr<Mesh> CreateSkyCube();
};
