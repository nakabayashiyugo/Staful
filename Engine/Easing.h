#pragma once
#include <iostream>
#include <cmath>
class Easing
{
	const float PIE = 3.141592;
public:
	Easing();
	float EaseInSine(float _value);
	float EaseOutCirc(float _value);
	float EaseInOutBack(float _value);
};

