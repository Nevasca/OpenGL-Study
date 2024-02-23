#pragma once

#include "Core.h"

class Shader;
class IndexBuffer;
class VertexArray;

class Renderer
{
public:

    void Clear() const;
    void Draw(const VertexArray& Va, const IndexBuffer& Ib, const Shader& Shader) const;
};
