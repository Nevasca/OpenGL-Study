#pragma once
#include <string>

class Shader;

namespace Rendering
{
    class UniformBuffer
    {
    public:
        UniformBuffer(const void* data, unsigned int size, const unsigned int bindingIndex, const std::string&& name, const bool bIsDynamic = false);
        ~UniformBuffer();

        void Bind() const;
        void Unbind() const;
        bool IsBound() const { return LastBoundUniformBufferId == m_RendererID; }
        void SetSubData(const void* data, unsigned int size, unsigned int offset = 0) const;
        void SetBindingIndexFor(const Shader& shader) const;

    private:

        static unsigned int LastBoundUniformBufferId;
        
        unsigned int m_RendererID{0};
        unsigned int m_BindingIndex{0};
        std::string m_Name;
    };
}
