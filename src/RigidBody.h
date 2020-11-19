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
};
#endif /* defined (__RIGID_BODY__) */