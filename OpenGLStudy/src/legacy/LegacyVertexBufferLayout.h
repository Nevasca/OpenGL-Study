#pragma once

#include <vector>

#include "OpenGLCore.h"

struct VertexBufferElement
{
    unsigned int Type;
    unsigned int Count;
    unsigned char IsNormalized;

    static unsigned int GetSizeOfType(unsigned int Type)
    {
        switch (Type)
        {
            case GL_FLOAT:          return 4;
            case GL_UNSIGNED_INT:   return 4;
            case GL_UNSIGNED_BYTE:  return 1;
        }

        ASSERT(false);
        return 0;
    }
};

class LegacyVertexBufferLayout
{
public:

    LegacyVertexBufferLayout()
        : m_Stride(0)
    { }

    template <typename T>
    void Push(unsigned int Count)
    {
        // static_assert(false);
    }

    template<>
    void Push<float>(unsigned int Count)
    {
        m_Elements.emplace_back(VertexBufferElement{GL_FLOAT, Count, GL_FALSE});
        m_Stride += Count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }

    template<>
    void Push<unsigned int>(unsigned int Count)
    {
        m_Elements.emplace_back(VertexBufferElement{GL_UNSIGNED_INT, Count, GL_FALSE});
        m_Stride += Count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    void Push<unsigned char>(unsigned int Count)
    {
        m_Elements.emplace_back(VertexBufferElement{GL_UNSIGNED_BYTE, Count, GL_TRUE});
        m_Stride += Count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    }

    inline unsigned int GetStride() const { return m_Stride; }
    inline const std::vector<VertexBufferElement>& GetElements() const {return m_Elements; }

private:

    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
};
