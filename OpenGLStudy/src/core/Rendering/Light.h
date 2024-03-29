#pragma once

struct Attenuation
{
    float Constant{1.f};
    float Linear{0.14f};
    float Quadratic{0.07f};
};

class Light
{
public:

    static Attenuation CalculateAttenuation(float range);
};
