#pragma once

namespace Glacirer
{
    namespace Rendering
    {
        class VertexBuffer
        {
        public:
            VertexBuffer(const void* data, unsigned int size, bool bIsDynamic = false);
            ~VertexBuffer();

            void Bind() const;
            void Unbind() const;
            void SetSubData(const void* data, unsigned int size) const;

        private:

            unsigned int m_RendererID{0};
        };
    }
}
