#pragma once
#ifndef __ADV_COLLISION_MANAGER__
#define __ADV_COLLISION_MANAGER__

#include "GameObject.h"
#include "ship.h"
#include <GLM/gtx/norm.hpp>

class AdvancedCollisionManager
{
public:
	static float SweptAABB(GameObject* obj1, GameObject* obj2, glm::vec2& normal);
	static float SweptAABB(GameObject* obj, glm::vec2& normal);
	
private:
	static float SweptRectRect(GameObject* obj1, GameObject* obj2, glm::vec2& normal);
	static float SweptCircleRect(GameObject* obj1, GameObject* obj2, glm::vec2& normal);
	static float SweptCircleCircle(GameObject* obj1, GameObject* obj2, glm::vec2& normal);

	static void ResolveCollision(GameObject* A, GameObject* B, glm::vec2 normal);
	static void PositionalCorrection(GameObject* A, GameObject* B, glm::vec2 normal, float penetration);
	
	AdvancedCollisionManager();
	~AdvancedCollisionManager();
};

typedef AdvancedCollisionManager ADCOMA;

#endif