#include "Rendering/Light.h"

namespace Glacirer
{
    namespace Rendering
    {
        Attenuation Light::CalculateAttenuation(float range)
        {
            Attenuation attenuation;
    
            // Linear and Quadratic values are based on this table:
            // https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
            // 4.5f and 75.f values get close to those values when formula is based on desired range
            attenuation.Linear = 4.5f / range;
            attenuation.Quadratic = 75.f / (range * range);
            attenuation.Constant = 1.f;

            return attenuation;
        }
    }
}
