#pragma once
#include "EngineAPI.h"
#include "Resolution.h"

namespace Glacirer
{
    namespace Rendering
    {
        class ENGINE_API Device
        {
        public:

            void SetViewportResolution(const Resolution& resolution) const;
            void Clear() const;
            void EnableDepthTest() const;
            void DisableDepthTest() const;
            void EnableDepthWrite() const;
            void DisableDepthWrite() const;
            void SetDepthFunction(const unsigned int function) const;
            void EnableStencilTest() const;
            void DisableStencilTest() const;
            void EnableStencilWrite() const;
            void DisableStencilWrite() const;
            void SetStencilFunction(const unsigned int function, const int reference, const unsigned int mask) const;
            void SetStencilOperation(const unsigned int fail, const unsigned int zFail, const unsigned int zPass) const;
            void EnableBlend() const;
            void DisableBlend() const;
            void SetBlendFunction(const unsigned int sourceFactor, const unsigned int destinationFactor) const;
            void EnableFaceCulling() const;
            void DisableFaceCulling() const;
            void SetCullingFaceFront() const;
            void SetCullingFaceBack() const;
            void SetCullingWindingOrder(bool bIsCounterClockwise) const;
            void EnableMSAA() const;
            void DisableMSAA() const;
            void EnableGammaCorrection() const;
            void DisableGammaCorrection() const;

            bool IsFaceCullingEnabled() const { return bIsFaceCullingEnabled; }

        private:

            mutable bool bIsFaceCullingEnabled{false};
        };
    }
}
