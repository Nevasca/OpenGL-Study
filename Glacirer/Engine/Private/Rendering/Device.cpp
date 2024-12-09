#include "Rendering/Device.h"
#include "Rendering/OpenGLCore.h"

namespace Glacirer
{
    namespace Rendering
    {
        void Device::SetViewportResolution(const Resolution& resolution) const
        {
            GLCall(glViewport(0, 0, static_cast<int>(resolution.Width), static_cast<int>(resolution.Height)));
        }

        void Device::Clear() const
        {
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
        }

        void Device::EnableDepthTest() const
        {
            GLCall(glEnable(GL_DEPTH_TEST));
        }

        void Device::DisableDepthTest() const
        {
            GLCall(glDisable(GL_DEPTH_TEST));
        }

        void Device::EnableDepthWrite() const
        {
            GLCall(glDepthMask(GL_TRUE));
        }

        void Device::DisableDepthWrite() const
        {
            GLCall(glDepthMask(GL_FALSE));
        }

        void Device::SetDepthFunction(const unsigned int function) const
        {
            GLCall(glDepthFunc(function));
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

        void Device::EnableBlend() const
        {
            GLCall(glEnable(GL_BLEND));
        }

        void Device::DisableBlend() const
        {
            GLCall(glDisable(GL_BLEND));
        }

        void Device::SetBlendFunction(const unsigned int sourceFactor, const unsigned int destinationFactor) const
        {
            GLCall(glBlendFunc(sourceFactor, destinationFactor));
        }

        void Device::EnableFaceCulling() const
        {
            GLCall(glEnable(GL_CULL_FACE));
            bIsFaceCullingEnabled = true;
        }

        void Device::DisableFaceCulling() const
        {
            GLCall(glDisable(GL_CULL_FACE));
            bIsFaceCullingEnabled = false;
        }

        // If you decide to cull front faces and you are using post processing
        // remember to set it back to back face before rendering the screen quad or you'll get a black screen
        // same when setting clockwise winding order
        void Device::SetCullingFaceFront() const
        {
            GLCall(glCullFace(GL_FRONT));
        }

        // OpenGL culls back faces by default
        // Call this method only if you have called SetCullingFaceFront() at some point
        void Device::SetCullingFaceBack() const
        {
            GLCall(glCullFace(GL_BACK));
        }

        void Device::SetCullingWindingOrder(bool bIsCounterClockwise) const
        {
            GLCall(glFrontFace(bIsCounterClockwise ? GL_CCW : GL_CW));
        }

        void Device::EnableMSAA() const
        {
            GLCall(glEnable(GL_MULTISAMPLE));
        }

        void Device::DisableMSAA() const
        {
            GLCall(glDisable(GL_MULTISAMPLE));
        }

        void Device::EnableGammaCorrection() const
        {
            GLCall(glEnable(GL_FRAMEBUFFER_SRGB)); 
        }

        void Device::DisableGammaCorrection() const
        {
            GLCall(glDisable(GL_FRAMEBUFFER_SRGB));
        }
    }
}
