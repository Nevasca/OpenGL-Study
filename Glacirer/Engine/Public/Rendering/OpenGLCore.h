#pragma once

#include <GL/glew.h>

// To use param as condition on macro, need to wrap on (), as (x)
#ifdef _DEBUG
    #define ASSERT(x) if (!(x)) __debugbreak()
#else
    #define ASSERT(x)
#endif

#ifdef _DEBUG
    #define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
    #define GLCall(x) x
#endif

namespace Glacirer
{
    namespace Rendering
    {
        void GLClearError();
        bool GLLogCall(const char* Function, const char* File, int Line);
    }
}
