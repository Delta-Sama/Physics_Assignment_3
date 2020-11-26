#include "PhysicsManager.h"

std::list<GameObject*> PhysicsManager::m_physical_objects;

PhysicsManager::PhysicsManager()
{
}

PhysicsManager::~PhysicsManager() = default;

void PhysicsManager::AddPhysicalObject(GameObject& obj)
{
	m_physical_objects.push_back(&obj);
}

float PhysicsManager::CheckWorldCollision(GameObject& obj)
{
	for (std::list<GameObject*>::iterator it = PhysicsManager::GetPhysicalObjects()->begin();
		it != PhysicsManager::GetPhysicalObjects()->end(); it++)
	{
		if (*it != &obj)
		{
			glm::vec2 normal;
			
			const float collision_time = ADCOMA::SweptAABB(&obj, *it, normal);

			/*if (collision_time < 1.0f)
			{
				std::cout << "Collision phy manag\n";
				obj.getRigidBody()->velocity.x *= obj.getRigidBody()->elasticity;
				obj.getRigidBody()->velocity.y *= obj.getRigidBody()->elasticity;
			}

			obj.getTransform()->position.x += obj.getRigidBody()->velocity.x * collision_time;
			obj.getTransform()->position.y += obj.getRigidBody()->velocity.y * collision_time;

			if (abs(normal.x) > 0.0001f) obj.getRigidBody()->velocity.x = -obj.getRigidBody()->velocity.x;
			if (abs(normal.y) > 0.0001f) obj.getRigidBody()->velocity.y = -obj.getRigidBody()->velocity.y;*/
			
		}
	}

	return 0.0f;
}
