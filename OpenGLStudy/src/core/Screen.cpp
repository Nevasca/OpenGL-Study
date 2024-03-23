#include "Screen.h"

int Screen::m_Width = 960;
int Screen::m_Height = 540;

void Screen::SetSize(int width, int height)
{
    m_Width = width;
    m_Height = height;
}
