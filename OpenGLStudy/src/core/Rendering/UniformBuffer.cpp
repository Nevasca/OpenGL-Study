#include "UniformBuffer.h"

#include <iostream>
#include <ostream>

#include "OpenGLCore.h"
#include "Shader.h"

namespace Rendering
{
    unsigned int UniformBuffer::LastBoundUniformBufferId = 0;
    
    UniformBuffer::UniformBuffer(const void* data, unsigned int size, const unsigned int bindingIndex, const std::string&& name, const bool bIsDynamic)
        : m_BindingIndex(bindingIndex), m_Name(name)
    {
        GLCall(glGenBuffers(1, &m_RendererID));
        Bind();
        GLCall(glBufferData(GL_UNIFORM_BUFFER, size, data, bIsDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
        GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, m_BindingIndex, m_RendererID));

        // We can also bind the uniform buffer point with range call, this way we could bind parts of a single
        // buffer to different points
        //GLCall(glBindBufferRange(GL_UNIFORM_BUFFER, m_BindingIndex, m_RendererID, 0, size));

        Unbind();
    }

    UniformBuffer::~UniformBuffer()
    {
        GLCall(glDeleteBuffers(1, &m_RendererID));

        if(IsBound())
        {
            LastBoundUniformBufferId = 0;
        }
    }

    void UniformBuffer::Bind() const
    {
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_RendererID));
        LastBoundUniformBufferId = m_RendererID;
    }

    void UniformBuffer::Unbind() const
    {
        GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
        LastBoundUniformBufferId = 0;
    }

    void UniformBuffer::SetSubData(const void* data, unsigned int size, unsigned int offset) const
    {
        ASSERT(IsBound());

        GLCall(glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data));
    }

    void UniformBuffer::SetBindingIndexFor(const Shader& shader) const
    {
        GLCall(unsigned int uniformBlockIndex = glGetUniformBlockIndex(shader.GetRendererID(), m_Name.c_str()));

        if(uniformBlockIndex == GL_INVALID_INDEX)
        {
            std::cout << "Warning: Uniform block '" << m_Name << "' does not exist on shader '" << shader.GetName() << "'\n";
            return;
        }
        
        GLCall(glUniformBlockBinding(shader.GetRendererID(), uniformBlockIndex, m_BindingIndex));
    }
}
