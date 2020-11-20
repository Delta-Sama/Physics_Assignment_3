#pragma once
#ifndef __MATHMANAGER_H__
#define __MATHMANAGER_H__

class MathManager
{
public:
	static constexpr float PI = 3.14159265359f;
	
	static float RadToDeg(float rad);
	static float DegToRad(float deg);

	static float To180Deg(float deg);
	
private:

	
};

typedef MathManager MAMA;

#endif