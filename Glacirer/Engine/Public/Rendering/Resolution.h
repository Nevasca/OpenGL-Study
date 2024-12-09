#pragma once

namespace Glacirer
{
    namespace Rendering
    {
        struct Resolution
        {
            unsigned int Width{0};
            unsigned int Height{0};

            Resolution() = default;
        
            Resolution(const unsigned int width, const unsigned int height)
            {
                Width = width;
                Height = height;
            }
        };
    };
}
