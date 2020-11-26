#include "AdvancedCollisionManager.h"

#include <algorithm>

#include "MathManager.h"
#include "PhysicsManager.h"
#include "Util.h"

AdvancedCollisionManager::AdvancedCollisionManager()
{
}

AdvancedCollisionManager::~AdvancedCollisionManager() = default;

float AdvancedCollisionManager::SweptAABB(GameObject* obj, glm::vec2& normal)
{
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

float AdvancedCollisionManager::SweptAABB(GameObject* obj1, GameObject* obj2, glm::vec2& normal)
{
	/*if (obj1->getShape() == CollisionShape::CIRCLE && obj2->getShape() == CollisionShape::RECTANGLE) // Circle-Rect
		return AdvancedCollisionManager::SweptCircleRect(obj1, obj2, normal);
	else if (obj1->getShape() == CollisionShape::RECTANGLE && obj2->getShape() == CollisionShape::CIRCLE) // Rect-Circle
		return AdvancedCollisionManager::SweptCircleRect(obj2, obj1, normal);
	else if (obj1->getShape() == CollisionShape::CIRCLE && obj2->getShape() == CollisionShape::CIRCLE) // Circle-Circle
		return AdvancedCollisionManager::SweptCircleCircle(obj2, obj1, normal);*/

	const float penetration = AdvancedCollisionManager::SweptCircleCircle(obj2, obj1, normal);
	ResolveCollision(obj1, obj2, normal);
	PositionalCorrection(obj1, obj2, normal, penetration);
	
	return 1.0f;
}

float AdvancedCollisionManager::SweptRectRect(GameObject* obj1, GameObject* obj2, glm::vec2& normal)
{
	return 1.0f;
}

float AdvancedCollisionManager::SweptCircleRect(GameObject* obj1, GameObject* obj2, glm::vec2& normal)
{
	return 1.0f;
}

float AdvancedCollisionManager::SweptCircleCircle(GameObject* obj1, GameObject* obj2, glm::vec2& normal)
{
	// Setup a couple pointers to each object
	GameObject* A = obj1;
	GameObject* B = obj2;
	
	// Vector from A to B
	glm::vec2 n = B->getTransform()->position - A->getTransform()->position;

	const float r = (float)A->getWidth() / 2.0f + (float)B->getWidth() / 2.0f;
	const float sq_r = r * r;
	
	const float sq_dist = n.x * n.x + n.y * n.y;

	if (sq_dist > sq_r) // No collision
		return 0.0f;

	// Circles have collided, now compute manifold
	float d = sqrt(sq_dist); // perform actual sqrt

	// If distance between circles is not zero
	if (d != 0.0f)
	{
		// Make the unit normal vector
		normal = n / d;
		std::cout << "Normal: " << normal.x << ", " << normal.y << "\n";
		Util::QueueLine(A->getTransform()->position, A->getTransform()->position + normal * 40.0f, { 1,0,0,1 });
		// Penetration is difference between radius and distance
		return r - d;
	}
	else // Circles are on same position
	{
		// Choose random (but consistent) values
		normal = glm::vec2(1, 0);
		return (float)A->getWidth() / 2.0f;
	}
}

void AdvancedCollisionManager::PositionalCorrection(GameObject* A, GameObject* B, glm::vec2 normal, float penetration)
{
	const float percent = 0.3f; // usually 20% to 80%
	const float slop = 0.05f; // usually 0.01 to 0.1
	glm::vec2 correction = (std::max(penetration - slop, 0.0f)
	/ (A->getRigidBody()->inverse_mass + B->getRigidBody()->inverse_mass)) * normal * percent;
	A->getTransform()->position -= A->getRigidBody()->inverse_mass * correction;
	B->getTransform()->position += B->getRigidBody()->inverse_mass * correction;
}

void AdvancedCollisionManager::ResolveCollision(GameObject* A, GameObject* B, glm::vec2 normal)
{
	// Calculate relative velocity
	const glm::vec2 relative_velocity = B->getRigidBody()->velocity - A->getRigidBody()->velocity;

	// Calculate relative velocity in terms of the normal direction
	
	const float velAlongNormal = MAMA::DotProduct(relative_velocity, normal);

	// Do not resolve if velocities are separating
	if (velAlongNormal > 0)
		return;
	
	std::cout << "Velocity along normal: " << velAlongNormal << "\n";
	
	// Calculate restitution
	const float e = std::min(A->getRigidBody()->elasticity, B->getRigidBody()->elasticity);

	// Calculate impulse scalar
	float j = -(1.0f + e) * velAlongNormal;
	j /= A->getRigidBody()->inverse_mass + B->getRigidBody()->inverse_mass;
	
	// Apply impulse
	const glm::vec2 impulse = normal * -j;
	
	A->getRigidBody()->velocity -= A->getRigidBody()->inverse_mass * impulse;
	B->getRigidBody()->velocity += B->getRigidBody()->inverse_mass * impulse;

	A->getTransform()->position.x += A->getRigidBody()->velocity.x;
	B->getTransform()->position.y += B->getRigidBody()->velocity.y;
	/*const float mass_sum = A->getRigidBody()->mass + B->getRigidBody()->mass;
	float ratio = A->getRigidBody()->mass / mass_sum;
	A->getRigidBody()->velocity -= ratio * impulse;

	ratio = B->getRigidBody()->mass / mass_sum;
	B->getRigidBody()->velocity += ratio * impulse;*/
}


/*float AdvancedCollisionManager::SweptRectRect(GameObject* obj1, GameObject* obj2, glm::vec2& normal)
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

	std::cout << "Collision found\n";
	
	if (xEntryTime > yEntryTime)
		(xEntryDist > 0.0f) ? normal.x = -1.0f : normal.x = 1.0f;
	else
		(yEntryDist > 0.0f) ? normal.y = -1.0f : normal.y = 1.0f;

	return entryTime;
}

float AdvancedCollisionManager::SweptCircleRect(GameObject* obj1, GameObject* obj2, glm::vec2& normal)
{
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
		normal.x = 0.0f;
		normal.y = 0.0f;
		return 1.0f;
	}

	// Determine the normal
	glm::vec2 dist;
	glm::vec2 border;
	
	if (abs(distX) > abs(distY))
	{
		normal.x = (distX > 0.0f ? -1.0f : 1.0f);
		normal.y = 0.0f;

		border = { 0, 1 };

		dist = { abs(movedPos1.x - (pos2.x + normal.x * size2.x / 2)) * -normal.x, 0.0f };
	}
	else
	{
		normal.x = 0.0f;
		normal.y = (distY > 0.0f ? -1.0f : 1.0f);
		
		border = { 1, 0 };
		
		dist = { 0.0f, abs(movedPos1.y - (pos2.y + normal.y * size2.y / 2)) * -normal.y };
	}


	float angle = acos((vel1.x * border.x + vel1.y * border.y) /
		(sqrt(vel1.x * vel1.x + vel1.y * vel1.y) * sqrt(border.x * border.x + border.y * border.y)));
	
	std::cout << "Normal: " << normal.x << ", " << normal.y << "\n";
	std::cout << "Angle: " << MAMA::RadToDeg(angle) << "\n";
	
	float odd_move = (radius + glm::dot(dist, normal)) / sin(angle);
	float speed = sqrt(vel1.x * vel1.x + vel1.y * vel1.y);

	std::cout << "odd/speed = " << odd_move / speed << "\n";

	return (1.0f - odd_move / speed);
}
*/