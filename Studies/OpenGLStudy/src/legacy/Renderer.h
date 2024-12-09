#pragma once

#include "OpenGLCore.h"

class LegacyIndexBuffer;
class LegacyShader;

class LegacyVertexArray;

class Renderer
{
public:

    void Clear() const;
    void Draw(const LegacyVertexArray& Va, const LegacyIndexBuffer& Ib, const LegacyShader& Shader) const;
};
