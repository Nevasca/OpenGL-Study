#pragma once

class VertexBuffer;
class VertexBufferLayout;

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& buffer, const VertexBufferLayout& layout);
    void Bind() const;
    void Unbind() const;

    unsigned int GetRendererID() const { return m_RendererID; }
    unsigned int GetNextAttributeLocation() const { return m_NextAttributeLocation; }
    void SetNextAttributeLocation(unsigned int nextAttributeLocation) { m_NextAttributeLocation = nextAttributeLocation; }
    void SetIsInstancedRenderingConfigured(bool bIsPrepared) { bIsInstancedRenderingConfigured = bIsPrepared; }
    bool IsInstancedRenderingConfigured() const { return bIsInstancedRenderingConfigured; }

private:

    static unsigned int m_LastBoundVertexArrayId;

    unsigned int m_RendererID;
    unsigned int m_NextAttributeLocation{0};
    bool bIsInstancedRenderingConfigured{false};
};
