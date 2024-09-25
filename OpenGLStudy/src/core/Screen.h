#pragma once
#include "Rendering/Resolution.h"

class Screen
{
public:

    static void SetSize(int width, int height);

    static int GetWidth() { return m_Width; }
    static int GetHeight() { return m_Height; }
    static Rendering::Resolution GetResolution();
    
private:

    static int m_Width;
    static int m_Height;
};
