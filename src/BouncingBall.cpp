#include "BouncingBall.h"
#include "CollisionManager.h"
#include "Config.h"

BouncingBall::BouncingBall(glm::vec2 pos)
{
	TextureManager::Instance()->load("../Assets/textures/Granade.png", "granade");

	setWidth(40);
	setHeight(40);

	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(BOUNCING_OBJECT);
	m_shape = BouncingShape::SQUARE;

	getTransform()->position = pos;
	getRigidBody()->velocity = glm::vec2(11.0f, 0.0f);
}

BouncingBall::~BouncingBall() = default;

void BouncingBall::update()
{
	getRigidBody()->velocity.y += -Config::g / Config::FPS;

	// Swept collision
	glm::vec2 normal;
	float collision_time = ADCOMA::SweptAABB(this, normal);

	if (collision_time < 1.0f)
	{
		getRigidBody()->velocity.x *= Config::ENERGY_LOSS;
		getRigidBody()->velocity.y *= Config::ENERGY_LOSS;
	}
	
	getTransform()->position.x += getRigidBody()->velocity.x * collision_time;
	getTransform()->position.y += getRigidBody()->velocity.y * collision_time;
	const float remainingtime = 1.0f - collision_time;

	if (remainingtime > 0.0f)
	{
		//getRigidBody()->velocity.x *= remainingtime;
		//getRigidBody()->velocity.y *= remainingtime;
		if (abs(normal.x) > 0.0001f) getRigidBody()->velocity.x = -getRigidBody()->velocity.x;
		if (abs(normal.y) > 0.0001f) getRigidBody()->velocity.y = -getRigidBody()->velocity.y;
	}
	
	
	// Set new position
}

void BouncingBall::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	const float ang = getTransform()->rotation.x;

	TextureManager::Instance()->draw("granade", x, y, ang, 255, true, SDL_FLIP_NONE, { getWidth(),getHeight() });

}

void BouncingBall::clean()
{
	
}
