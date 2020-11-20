#include "MathManager.h"
#include <algorithm>

float MathManager::RadToDeg(float rad)
{
	return ((rad / MAMA::PI) * 180.0f);
}

float MathManager::DegToRad(float deg)
{
	return ((deg / 180.0f) * MAMA::PI);
}

float MathManager::To180Deg(float deg)
{
	while (abs(deg) > 360.0f)
	{
		deg += 360.0f * ( deg > 0 ? -1 : 1 );
	}
	if (abs(deg) > 180.0f)
	{
		deg += 360.0f * (deg > 0 ? -1 : 1);
	}
	return deg;
}
