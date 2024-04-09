#include "Easing.h"

Easing::Easing()
{
}

float Easing::EaseInSine(float _value)
{
	return 1 - cos((_value * PIE) / 2);
}

float Easing::EaseOutCirc(float _value)
{
	return  sqrt(1 - pow(_value - 1, 2));
}

float Easing::EaseInOutBack(float _value)
{
	const float c1 = 1.70158;
	const float c2 = c1 * 1.525;

	if (_value < 0.5)
	{
		return (pow(2 * _value, 2) * ((c2 + 1) * 2 * _value - c2)) / 2;
	}
	else
	{
		return (pow(2 * _value - 2, 2) * ((c2 + 1) * (_value * 2 - 2) + c2) + 2) / 2;
	}
}
