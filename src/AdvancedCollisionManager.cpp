#include "AdvancedCollisionManager.h"

#include <algorithm>

#include "MathManager.h"
#include "PhysicsManager.h"
#include "Util.h"

AdvancedCollisionManager::AdvancedCollisionManager()
{
}

AdvancedCollisionManager::~AdvancedCollisionManager() = default;

Manifold AdvancedCollisionManager::SweptAABB(GameObject* obj)
{
	Manifold result;
	result.A = obj;
	result.B = nullptr;
	
	float xEntryDist, yEntryDist;
	float xExitDist, yExitDist;

	const float l = std::numeric_limits<float>::infinity();
	
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
		yEntryDist = Config::BOTTOM_BORDER - (pos1.y + size1.y / 2);
		yExitDist = (Config::BOTTOM_BORDER + l) - (pos1.y + size1.y / 2);
	}
	else {
		yEntryDist = 0 - (pos1.y - size1.y / 2);
		yExitDist = -l - (pos1.y - size1.y / 2);
	}

	float xEntryTime, yEntryTime;

	// Time to reach the closest edge
	if (vel1.x == 0.0f) {
		xEntryTime = std::numeric_limits<float>::infinity();
	}
	else {
		xEntryTime = xEntryDist / vel1.x;
	}

	if (vel1.y == 0.0f) {
		yEntryTime = std::numeric_limits<float>::infinity();
	}
	else {
		yEntryTime = yEntryDist / vel1.y;
	}

	const float entryTime = std::min(xEntryTime, yEntryTime);

	// No collision case
	if (xEntryTime < 0.0f && yEntryTime < 0.0f || xEntryTime > 1.0f && yEntryTime > 1.0f)
	{
		result.normal.x = 0.0f;
		result.normal.y = 0.0f;
		result.collisionTime = 1.0f;
		return result;
	}
	
	// Collision was found
	result.normal.x = 0.0f;
	result.normal.y = 0.0f;

	if (xEntryTime < yEntryTime)
		(xEntryDist > 0.0f) ? result.normal.x = -1.0f : result.normal.x = 1.0f;
	else
		(yEntryDist > 0.0f) ? result.normal.y = -1.0f : result.normal.y = 1.0f;

	result.collisionTime = entryTime;
	
	return result;
}

Manifold AdvancedCollisionManager::SweptAABB(GameObject* obj1, GameObject* obj2)
{
	Manifold result;
	
	if (obj1->getShape() == CollisionShape::CIRCLE && obj2->getShape() == CollisionShape::RECTANGLE) // Circle-Rect
		result = AdvancedCollisionManager::SweptCircleRect(obj1, obj2);
	else if (obj1->getShape() == CollisionShape::RECTANGLE && obj2->getShape() == CollisionShape::CIRCLE) // Rect-Circle
		result = AdvancedCollisionManager::SweptCircleRect(obj2, obj1);
	else
		result = AdvancedCollisionManager::SweptRectRect(obj2, obj1);

	return result;
}

Manifold AdvancedCollisionManager::SweptRectRect(GameObject* obj1, GameObject* obj2)
{
	Manifold result;
	result.A = obj1;
	result.B = obj2;
	
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
		result.normal.x = 0.0f;
		result.normal.y = 0.0f;
		result.collisionTime = 1.0f;
		return result;
	}

	// Collision was found
	result.normal.x = 0.0f;
	result.normal.y = 0.0f;

	std::cout << "Collision found\n";
	
	if (xEntryTime > yEntryTime)
		(xEntryDist > 0.0f) ? result.normal.x = -1.0f : result.normal.x = 1.0f;
	else
		(yEntryDist > 0.0f) ? result.normal.y = -1.0f : result.normal.y = 1.0f;

	result.collisionTime = entryTime;
	
	return result;
}

Manifold AdvancedCollisionManager::SweptCircleRect(GameObject* obj1, GameObject* obj2)
{
	Manifold result;
	result.A = obj1;
	result.B = obj2;
	
	const glm::vec2 vel1 = obj1->getRigidBody()->velocity;
	const glm::vec2 vel2 = obj1->getRigidBody()->velocity;

	const glm::vec2 pos1 = obj1->getTransform()->position;
	const glm::vec2 pos2 = obj2->getTransform()->position;

	const glm::vec2 movedPos1 = pos1 + vel1;
	const glm::vec2 movedPos2 = pos2 + vel2;

	const glm::vec2 size1 = glm::vec2(obj1->getWidth(), obj1->getHeight());
	const glm::vec2 size2 = glm::vec2(obj2->getWidth(), obj2->getHeight());

	const float radius = size1.x / 2;
	
	float testX = movedPos1.x;
	float testY = movedPos1.y;

	if (pos1.x < pos2.x - size2.x / 2) testX = pos2.x - size2.x / 2; // left edge
	else if (pos1.x > pos2.x + size2.x / 2) testX = pos2.x + size2.x / 2; // right edge

	if (pos1.y < pos2.y - size2.y / 2) testY = pos2.y - size2.y / 2; // top edge
	else if (pos1.y > pos2.y + size2.y / 2) testY = pos2.y + size2.y / 2; // bottom edge

	const float distX = testX - pos1.x;
	const float distY = testY - pos1.y;
	const float movedDistX = testX - movedPos1.x;
	const float movedDistY = testY - movedPos1.y;
	const float sq_distance = (movedDistX * movedDistX) + (movedDistY * movedDistY);
	
	// If no collision
	if (sq_distance > radius * radius)
	{
		result.normal.x = 0.0f;
		result.normal.y = 0.0f;
		result.collisionTime = 1.0f;
		return result;
	}
	
	// Determine the normal
	glm::vec2 dist;
	glm::vec2 border;
	
	if (abs(distX) > abs(distY))
	{
		result.normal.x = (distX > 0.0f ? -1.0f : 1.0f);
		result.normal.y = 0.0f;

		border = { 0, 1 };

		dist = { abs(movedPos1.x - (movedPos2.x + result.normal.x * size2.x / 2)) * -result.normal.x, 0.0f };
	}
	else
	{
		result.normal.x = 0.0f;
		result.normal.y = (distY > 0.0f ? -1.0f : 1.0f);
		
		border = { 1, 0 };
		
		dist = { 0.0f, abs(movedPos1.y - (movedPos2.y + result.normal.y * size2.y / 2)) * -result.normal.y };
	}


	float angle = acos((vel1.x * border.x + vel1.y * border.y) /
		(sqrt(vel1.x * vel1.x + vel1.y * vel1.y) * sqrt(border.x * border.x + border.y * border.y)));
	
	float odd_move = (radius + glm::dot(dist, result.normal)) / sin(angle);
	float speed = sqrt(vel1.x * vel1.x + vel1.y * vel1.y);
	//std::cout << "odd_move: " << odd_move << "\n";
	result.collisionTime = std::clamp(1.0f - odd_move / speed,0.0f,0.99f);

	return result;
}