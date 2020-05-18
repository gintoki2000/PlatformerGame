#include "Math.h"
#include <cstdlib>

float Math::Lerp(float current, float target, float step)
{
    return (target - current) * step;
}

float Math::Sign(float v)
{
    if (v == 0.f)
        return 0.f;
    if (v < 0.f)
        return -1.f;
    else
        return 1.f;
}

int Math::Rand(int a, int b)
{
	return std::rand() % (b - a) + a;
}
