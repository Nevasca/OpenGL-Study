#pragma once

class Screen
{
public:

    static void SetSize(int width, int height);

    static int GetWidth() { return m_Width; }
    static int GetHeight() { return m_Height; }
    
private:

    static int m_Width;
    static int m_Height;
};
