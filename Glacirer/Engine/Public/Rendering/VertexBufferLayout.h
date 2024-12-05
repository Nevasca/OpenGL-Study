#pragma once

#include <memory>
#include <vector>

#include <glm/glm.hpp>

namespace Rendering
{
    struct VertexBufferAttribute
    {
        VertexBufferAttribute(unsigned int count, bool isNormalized, unsigned int divisor)
            : Count(count), bIsNormalized(isNormalized), Divisor(divisor)
        { }

        virtual ~VertexBufferAttribute() = default;

        virtual unsigned int CreateAttribute(unsigned int location, unsigned int offset, unsigned int stride) = 0;
        virtual unsigned int GetSize() const = 0;

        unsigned int Count{0};
        bool bIsNormalized{false};
        unsigned int Divisor;
    };

    struct VertexBufferFloatAttribute : public VertexBufferAttribute
    {
        VertexBufferFloatAttribute(unsigned int count, bool isNormalized, unsigned int divisor)
            : VertexBufferAttribute(count, isNormalized, divisor)
        { }

        unsigned int CreateAttribute(unsigned int location, unsigned int offset, unsigned int stride) override;
        unsigned int GetSize() const override { return 4; }
    };

    struct VertexBufferUnsignedIntAttribute : public VertexBufferAttribute
    {
        VertexBufferUnsignedIntAttribute(unsigned int count, bool isNormalized, unsigned int divisor)
            : VertexBufferAttribute(count, isNormalized, divisor)
        { }

        unsigned int CreateAttribute(unsigned location, unsigned int offset, unsigned int stride) override;
        unsigned int GetSize() const override { return 4; }
    };

    struct VertexBufferUnsignedByteAttribute : public VertexBufferAttribute
    {
        VertexBufferUnsignedByteAttribute(unsigned int count, bool isNormalized, unsigned int divisor)
            : VertexBufferAttribute(count, isNormalized, divisor)
        { }
        
        unsigned int CreateAttribute(unsigned location, unsigned int offset, unsigned int stride) override;
        unsigned int GetSize() const override { return 1; }
    };

    struct VertexBufferMat4Attribute : public VertexBufferAttribute
    {
        VertexBufferMat4Attribute(unsigned int count, bool isNormalized, unsigned int divisor)
            : VertexBufferAttribute(count, isNormalized, divisor)
        { }

        unsigned int CreateAttribute(unsigned location, unsigned int offset, unsigned int stride) override;
        unsigned int GetSize() const override { return sizeof(glm::mat4); }
    };
}

class VertexBufferLayout
{
public:

    unsigned int CreateAttributes(unsigned int firstAttributeLocation = 0) const;

    void PushFloat(unsigned int count, unsigned int divisor = 0)
    {
        auto floatAttribute = std::make_unique<Rendering::VertexBufferFloatAttribute>(count, false, divisor);
        m_Stride += count * floatAttribute->GetSize();
        
        m_Elements.emplace_back(std::move(floatAttribute));
    }

    void PushUnsignedInt(unsigned int count, unsigned int divisor = 0)
    {
        auto unsignedIntAttribute = std::make_unique<Rendering::VertexBufferUnsignedIntAttribute>(count, false, divisor);
        m_Stride += count * unsignedIntAttribute->GetSize();
        
        m_Elements.emplace_back(std::move(unsignedIntAttribute));
    }

    void PushUnsignedChar(unsigned int count, unsigned int divisor = 0)
    {
        auto unsignedByteAttribute = std::make_unique<Rendering::VertexBufferUnsignedByteAttribute>(count, true, divisor);
        m_Stride += count * unsignedByteAttribute->GetSize();
        
        m_Elements.emplace_back(std::move(unsignedByteAttribute));
    }

    void PushMat4(unsigned int divisor = 0)
    {
        auto mat4Attribute = std::make_unique<Rendering::VertexBufferMat4Attribute>(1, false, divisor);
        m_Stride += mat4Attribute->GetSize();

        m_Elements.emplace_back(std::move(mat4Attribute));
    }

    unsigned int GetStride() const { return m_Stride; }

private:

    std::vector<std::unique_ptr<Rendering::VertexBufferAttribute>> m_Elements;
    unsigned int m_Stride{0};
};
