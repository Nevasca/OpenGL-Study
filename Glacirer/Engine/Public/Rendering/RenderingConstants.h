#pragma once

namespace Glacirer
{
    namespace Rendering
    {
        static constexpr int MAX_SKYBOXES = 1;

        static constexpr int MAX_DIRECTIONAL_LIGHTS = 3;
        static constexpr int MAX_POINT_LIGHTS = 20;
        static constexpr int MAX_SPOT_LIGHTS = 20;
        
        static constexpr unsigned int TOTAL_SYSTEM_RESERVED_TEXTURE_SLOTS = MAX_SKYBOXES + MAX_DIRECTIONAL_LIGHTS + MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS;
    }
}
