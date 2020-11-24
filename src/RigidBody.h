#pragma once
#ifndef __RIGID_BODY__
#define __RIGID_BODY__
#include <glm/vec2.hpp>

struct RigidBody
{
	float mass;
	glm::vec2 velocity;
	glm::vec2 acceleration;
	glm::vec2 direction;
	glm::vec2 force;
	bool isColliding;

	glm::vec2 getMomentum()
	{
		return glm::vec2(mass * velocity.x, mass * velocity.y);
	}
};
#endif /* defined (__RIGID_BODY__) */