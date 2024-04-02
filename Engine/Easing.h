#pragma once
#include <iostream>
#include <cmath>
class Easing
{
	const float PIE = 3.141592;
public:
	Easing();
	float EaseInSine(float pos);
	float EaseInOutBack(float pos);
};

