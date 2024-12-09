#include "Screen.h"

namespace Glacirer
{
    int Screen::m_Width = 960;
    int Screen::m_Height = 540;

    void Screen::SetSize(int width, int height)
    {
        m_Width = width;
        m_Height = height;
    }

    Rendering::Resolution Screen::GetResolution()
    {
        return Rendering::Resolution{static_cast<unsigned int>(m_Width), static_cast<unsigned int>(m_Height)};
    }

    float Screen::GetAspectRatio()
    {
        return static_cast<float>(m_Width) / static_cast<float>(m_Height);
    }
}
