#include "Device.h"
#include "OpenGLCore.h"

namespace Rendering
{
    void Device::EnableDepthTest() const
    {
        GLCall(glEnable(GL_DEPTH_TEST));
    }

    void Device::DisableDepthTest() const
    {
        GLCall(glDisable(GL_DEPTH_TEST));
    }

    void Device::EnableStencilTest() const
    {
        GLCall(glEnable(GL_STENCIL_TEST));
    }

    void Device::DisableStencilTest() const
    {
        GLCall(glDisable(GL_STENCIL_TEST));
    }

    void Device::EnableStencilWrite() const
    {
        // Mask that will be ANDed with value about to be written on stencil buffer
        // 0xFF: each bit is written as is  
        GLCall(glStencilMask(0xFF));
    }

    void Device::DisableStencilWrite() const
    {
        //0x00: each bit turns into 0 in the stencil buffer, disabling writes
        GLCall(glStencilMask(0x00));
    }

    void Device::SetStencilFunction(const unsigned int function, const int reference, const unsigned int mask) const
    {
        GLCall(glStencilFunc(function, reference, mask));
    }

    void Device::SetStencilOperation(const unsigned int fail, const unsigned int zFail, const unsigned int zPass) const
    {
        GLCall(glStencilOp(fail, zFail, zPass));
    }
}
