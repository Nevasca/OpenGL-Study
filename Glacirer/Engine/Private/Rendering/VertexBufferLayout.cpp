#include "Rendering/VertexBufferLayout.h"

#include "Rendering/OpenGLCore.h"

namespace Rendering
{
    unsigned int VertexBufferFloatAttribute::CreateAttribute(unsigned int location, unsigned int offset, unsigned int stride)
    {
        GLCall(glEnableVertexAttribArray(location)); 
        GLCall(glVertexAttribPointer(location, Count, GL_FLOAT, bIsNormalized ? GL_TRUE : GL_FALSE, stride, (const void*)offset));

        if(Divisor > 0)
        {
            GLCall(glVertexAttribDivisor(location, Divisor));
        }

        return location + 1;
    }

    unsigned int VertexBufferUnsignedIntAttribute::CreateAttribute(unsigned int location, unsigned int offset, unsigned int stride)
    {
        GLCall(glEnableVertexAttribArray(location)); 
        GLCall(glVertexAttribPointer(location, Count, GL_UNSIGNED_INT, bIsNormalized ? GL_TRUE : GL_FALSE, stride, (const void*)offset));

        if(Divisor > 0)
        {
            GLCall(glVertexAttribDivisor(location, Divisor));
        }

        return location + 1;
    }

    unsigned int VertexBufferUnsignedByteAttribute::CreateAttribute(unsigned int location, unsigned int offset, unsigned int stride)
    {
        GLCall(glEnableVertexAttribArray(location)); 
        GLCall(glVertexAttribPointer(location, Count, GL_UNSIGNED_BYTE, bIsNormalized ? GL_TRUE : GL_FALSE, stride, (const void*)offset));

        if(Divisor > 0)
        {
            GLCall(glVertexAttribDivisor(location, Divisor));
        }

        return location + 1;
    }

    unsigned int VertexBufferMat4Attribute::CreateAttribute(unsigned int location, unsigned int offset, unsigned int stride)
    {
        // Set attribute pointers for model matrix(4 times vec4)
        GLCall(glEnableVertexAttribArray(location));
        GLCall(glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, stride, (void*)offset));
        
        GLCall(glEnableVertexAttribArray(location + 1));
        GLCall(glVertexAttribPointer(location + 1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(offset + sizeof(glm::vec4))));
        
        GLCall(glEnableVertexAttribArray(location + 2));
        GLCall(glVertexAttribPointer(location + 2, 4, GL_FLOAT, GL_FALSE, stride, (void*)(offset + 2 * sizeof(glm::vec4))));
        
        GLCall(glEnableVertexAttribArray(location + 3));
        GLCall(glVertexAttribPointer(location + 3, 4, GL_FLOAT, GL_FALSE, stride, (void*)(offset + 3 * sizeof(glm::vec4))));

        if(Divisor > 0)
        {
            GLCall(glVertexAttribDivisor(location, Divisor));
            GLCall(glVertexAttribDivisor(location + 1, Divisor));
            GLCall(glVertexAttribDivisor(location + 2, Divisor));
            GLCall(glVertexAttribDivisor(location + 3, Divisor));
        }

        return location + 4;
    }
}

unsigned int VertexBufferLayout::CreateAttributes(unsigned int firstAttributeLocation) const
{
    unsigned int offset = 0;
    unsigned int nextAttributeLocation = firstAttributeLocation;

    for (const auto& element : m_Elements)
    {
        nextAttributeLocation = element->CreateAttribute(nextAttributeLocation, offset, m_Stride);
        
        offset += element->Count * element->GetSize();
    }

    return nextAttributeLocation;
}
