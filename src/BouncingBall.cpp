#include "BouncingBall.h"

#include <algorithm>

#include "CollisionManager.h"
#include "Config.h"
#include "PhysicsManager.h"

BouncingBall::BouncingBall(glm::vec2 pos)
{
	TextureManager::Instance()->load("../Assets/textures/BasketballBall.png", "ball");
	TextureManager::Instance()->load("../Assets/textures/BasketballBallShadow.png", "shadow");
	
	setWidth(40);
	setHeight(40);

	getRigidBody()->elasticity = 0.85f;
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	getRigidBody()->SetMass(1.0f);
	
	setType(BOUNCING_OBJECT);
	setShape(CollisionShape::CIRCLE);

	getTransform()->position = pos;
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
}

BouncingBall::~BouncingBall() = default;

void BouncingBall::update()
{
	getTransform()->rotation.x += std::clamp(getRigidBody()->velocity.x,
		-Config::MAX_ROTATION, Config::MAX_ROTATION);

	//getRigidBody()->velocity.y += -Config::g / Config::FPS;

	// Swept collision
	PhysicsManager::CheckWorldCollision(*this);
	
	const struct Manifold result = ADCOMA::SweptAABB(this);
	CollisionResponse(result.collisionTime, result.normal);
	
}

void BouncingBall::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	const float ang = getTransform()->rotation.x;

	float shadow_w = getWidth() * (getTransform()->position.y / Config::BOTTOM_BORDER);
	
	TextureManager::Instance()->draw("shadow", x, Config::BOTTOM_BORDER - getHeight() / 2, 0, 255, true,
		SDL_FLIP_NONE, { shadow_w,getHeight() });
	TextureManager::Instance()->draw("ball", x, y, ang, 255, true, SDL_FLIP_NONE, { getWidth(),getHeight() });
}

void BouncingBall::clean()
{
	
}

void BouncingBall::CollisionResponse(float collision_time, glm::vec2 normal)
{
	if (collision_time < 1.0f)
	{
		getRigidBody()->velocity.x *= getRigidBody()->elasticity;
		getRigidBody()->velocity.y *= getRigidBody()->elasticity;
	}

	getTransform()->position.x += getRigidBody()->velocity.x * collision_time;
	getTransform()->position.y += getRigidBody()->velocity.y * collision_time;

	if (abs(normal.x) > 0.0001f) getRigidBody()->velocity.x = -getRigidBody()->velocity.x;
	if (abs(normal.y) > 0.0001f) getRigidBody()->velocity.y = -getRigidBody()->velocity.y;
}
