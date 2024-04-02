#include "Easing.h"

Easing::Easing()
{
}

float Easing::EaseInSine(float pos)
{
	return 1 - cos((pos * PIE) / 2);
}

float Easing::EaseInOutBack(float pos)
{
	const float c1 = 1.70158;
	const float c2 = c1 * 1.525;

	if (pos < 0.5)
	{
		return (pow(2 * pos, 2) * ((c2 + 1) * 2 * pos - c2)) / 2;
	}
	else
	{
		return (pow(2 * pos - 2, 2) * ((c2 + 1) * (pos * 2 - 2) + c2) + 2) / 2;
	}
}
