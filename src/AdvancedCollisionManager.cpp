#include "AdvancedCollisionManager.h"

#include <algorithm>

AdvancedCollisionManager::AdvancedCollisionManager()
{
}

AdvancedCollisionManager::~AdvancedCollisionManager() = default;

float AdvancedCollisionManager::SweptAABB(GameObject* obj1, GameObject* obj2, glm::vec2& normal)
{
	float xEntryDist, yEntryDist;
	float xExitDist, yExitDist;

	const glm::vec2 vel1 = obj1->getRigidBody()->velocity;
	
	const glm::vec2 pos1 = obj1->getTransform()->position;
	const glm::vec2 pos2 = obj2->getTransform()->position;

	const glm::vec2 size1 = glm::vec2(obj1->getWidth(), obj1->getHeight());
	const glm::vec2 size2 = glm::vec2(obj2->getWidth(), obj2->getHeight());

	// Distance between the object and the nearest edge
	if (vel1.x > 0.0f) {
		xEntryDist = pos2.x - (pos1.x + size1.x);
		xExitDist = (pos2.x + size2.x) - pos1.x;
	}
	else {
		xEntryDist = (pos2.x + size2.x) - pos1.x;
		xExitDist = pos2.x - (pos1.x + size1.x);
	}

	if (vel1.y > 0.0f) {
		yEntryDist = pos2.y - (pos1.y + size1.y);
		yExitDist = (pos2.y + size2.y) - pos1.y;
	}
	else {
		yEntryDist = (pos2.y + size2.y) - pos1.y;
		yExitDist = pos2.y - (pos1.y + size1.y);
	}

	float xEntryTime, yEntryTime;
	float xExitTime, yExitTime;

	// Time to reach the closest edge
	if (vel1.x == 0.0f) {
		xEntryTime = -std::numeric_limits<float>::infinity();
		xExitTime = std::numeric_limits<float>::infinity();
	}
	else {
		xEntryTime = xEntryDist / vel1.x;
		xExitTime = xExitDist / vel1.x;
	}

	if (vel1.y == 0.0f) {
		yEntryTime = -std::numeric_limits<float>::infinity();
		yExitTime = std::numeric_limits<float>::infinity();
	}
	else {
		yEntryTime = yEntryDist / vel1.y;
		yExitTime = yExitDist / vel1.y;
	}

	const float entryTime = std::max(xEntryTime, yEntryTime);
	const float exitTime = std::min(xExitTime, yExitTime);

	// No collision case
	if (entryTime > exitTime || xEntryTime < 0.0f && yEntryTime < 0.0f || xEntryTime > 1.0f || yEntryTime > 1.0f)
	{
		normal.x = 0.0f;
		normal.y = 0.0f;
		return 1.0f;
	}

	// Collision was found
	normal.x = 0.0f;
	normal.y = 0.0f;
	
	if (xEntryTime > yEntryTime)
		(xEntryDist > 0.0f) ? normal.x = -1.0f : normal.x = 1.0f;
	else
		(yEntryDist > 0.0f) ? normal.y = -1.0f : normal.y = 1.0f;

	return entryTime;
}

float AdvancedCollisionManager::SweptAABB(GameObject* obj, glm::vec2& normal)
{
	float xEntryDist, yEntryDist;
	float xExitDist, yExitDist;

	const float l = 256.0f;
	
	const glm::vec2 vel1 = obj->getRigidBody()->velocity;
	const glm::vec2 pos1 = obj->getTransform()->position;
	const glm::vec2 size1 = glm::vec2(obj->getWidth(), obj->getHeight());

	// Distance between the object and the nearest edge
	if (vel1.x > 0.0f) {
		xEntryDist = Config::SCREEN_WIDTH - (pos1.x + size1.x / 2);
		xExitDist = Config::SCREEN_WIDTH + l - (pos1.x + size1.x / 2);
	}
	else {
		xEntryDist = 0 - (pos1.x - size1.x / 2);
		xExitDist = -l - (pos1.x - size1.x / 2);
	}

	if (vel1.y > 0.0f) {
		yEntryDist = Config::SCREEN_HEIGHT - (pos1.y + size1.y / 2);
		yExitDist = (Config::SCREEN_HEIGHT + l) - (pos1.y + size1.y / 2);
	}
	else {
		yEntryDist = 0 - (pos1.y - size1.y / 2);
		yExitDist = -l - (pos1.y - size1.y / 2);
	}

	float xEntryTime, yEntryTime;
	float xExitTime, yExitTime;

	// Time to reach the closest edge
	if (vel1.x == 0.0f) {
		xEntryTime = -std::numeric_limits<float>::infinity();
		xExitTime = std::numeric_limits<float>::infinity();
	}
	else {
		xEntryTime = xEntryDist / vel1.x;
		xExitTime = xExitDist / vel1.x;
	}

	if (vel1.y == 0.0f) {
		yEntryTime = -std::numeric_limits<float>::infinity();
		yExitTime = std::numeric_limits<float>::infinity();
	}
	else {
		yEntryTime = yEntryDist / vel1.y;
		yExitTime = yExitDist / vel1.y;
	}

	const float entryTime = std::min(xEntryTime, yEntryTime);

	// No collision case
	if (xEntryTime < 0.0f && yEntryTime < 0.0f || xEntryTime > 1.0f && yEntryTime > 1.0f)
	{
		normal.x = 0.0f;
		normal.y = 0.0f;
		return 1.0f;
	}
	
	// Collision was found
	normal.x = 0.0f;
	normal.y = 0.0f;

	if (xEntryTime < yEntryTime)
		(xEntryDist > 0.0f) ? normal.x = -1.0f : normal.x = 1.0f;
	else
		(yEntryDist > 0.0f) ? normal.y = -1.0f : normal.y = 1.0f;

	return entryTime;
}
