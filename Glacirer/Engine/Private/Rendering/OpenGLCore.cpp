#include "Rendering/OpenGLCore.h"

#include <iostream>

namespace Glacirer
{
    namespace Rendering
    {
        void GLClearError()
        {
            while(glGetError() != GL_NO_ERROR);

            // GL_NO_ERROR is 0, we could write like this:
            // while(!glGetError());
        }

        bool GLLogCall(const char* Function, const char* File, int Line)
        {
            while(GLenum error = glGetError())
            {
                std::cout << "[OpenGL Error] (" << error << "): " << Function << " " << File << ":" << Line << "\n";
                return false;
            }

            return true;
        }
    }
}
