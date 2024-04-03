#pragma once

class IndexBuffer
{
public:

    IndexBuffer(const unsigned int* Data, unsigned int Count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    unsigned int GetCount() const { return m_Count; }

private:

    static unsigned int m_LastBoundIndexBufferId;
    
    unsigned int m_RendererID{0};
    unsigned int m_Count;
};
