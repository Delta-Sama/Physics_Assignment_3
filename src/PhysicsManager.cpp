#include "PhysicsManager.h"

#include <algorithm>

#include "Util.h"

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
			static glm::vec2 last_normal = {0.0f,0.0f};
			
			const Manifold result = ADCOMA::SweptAABB(&obj, *it);

			const float e = std::min(result.A->getRigidBody()->elasticity,result.B->getRigidBody()->elasticity);
			
			obj.getTransform()->position.x += obj.getRigidBody()->velocity.x * result.collisionTime;
			obj.getTransform()->position.y += obj.getRigidBody()->velocity.y * result.collisionTime;

			if (abs(result.normal.x) > 0.0001f)
			{
				bool opposite_vel = (obj.getRigidBody()->velocity.x >= 0 && (*it)->getRigidBody()->velocity.x <= 0)
				|| (obj.getRigidBody()->velocity.x <= 0 && (*it)->getRigidBody()->velocity.x >= 0);
				
				if (opposite_vel) // Moving toward each other
				{
					obj.getRigidBody()->velocity.x = -obj.getRigidBody()->velocity.x;
					(*it)->getRigidBody()->velocity.x = -(*it)->getRigidBody()->velocity.x;
				}
				else // One is catching up another
				{
					if (obj.getRigidBody()->mass > (*it)->getRigidBody()->mass)
						(*it)->getRigidBody()->velocity.x = -(*it)->getRigidBody()->velocity.x;
					else
						obj.getRigidBody()->velocity.x = -obj.getRigidBody()->velocity.x;
				}
			}
			if (abs(result.normal.y) > 0.0001f)
			{
				bool opposite_vel = (obj.getRigidBody()->velocity.y >= 0 && (*it)->getRigidBody()->velocity.y <= 0)
					|| (obj.getRigidBody()->velocity.y <= 0 && (*it)->getRigidBody()->velocity.y >= 0);

				if (opposite_vel) // Moving toward each other
				{
					obj.getRigidBody()->velocity.y = -obj.getRigidBody()->velocity.y;
					(*it)->getRigidBody()->velocity.y = -(*it)->getRigidBody()->velocity.y;
				}
				else // One is catching up another
				{
					if (obj.getRigidBody()->mass > (*it)->getRigidBody()->mass)
						(*it)->getRigidBody()->velocity.y = -(*it)->getRigidBody()->velocity.y;
					else
						obj.getRigidBody()->velocity.y = -obj.getRigidBody()->velocity.y;
				}
			}

			(*it)->getTransform()->position.x += (*it)->getRigidBody()->velocity.x;
			(*it)->getTransform()->position.y += (*it)->getRigidBody()->velocity.y;
			
			if (result.collisionTime < 1.0f)
			{
				last_normal = result.normal;
				/*obj.getRigidBody()->velocity.x *= result.A->getRigidBody()->elasticity;
				obj.getRigidBody()->velocity.y *= result.A->getRigidBody()->elasticity;

				result.B->getRigidBody()->velocity.x *= result.B->getRigidBody()->elasticity;
				result.B->getRigidBody()->velocity.y *= result.B->getRigidBody()->elasticity;*/

				
			}

			Util::QueueCircle({ 800.0f,350.0f }, 2, { 0,0,1,1 });
			Util::QueueLine({ 800.0f,350.0f }, glm::vec2(800.0f, 350.0f) + last_normal * 20.0f, { 0,0,1,1 });
			
		}
	}

	return 0.0f;
}
