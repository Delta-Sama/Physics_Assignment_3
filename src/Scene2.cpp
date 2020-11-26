#include "Scene2.h"

#include "Game.h"
#include "EventManager.h"
#include "MathManager.h"
#include "PhysicsManager.h"
#include "Util.h"

Scene2::Scene2()
{
	Scene2::start();
}

Scene2::~Scene2()
= default;

// SCENE

void Scene2::start()
{
	m_ball = new BouncingBall(glm::vec2(500.0f,100.0f));
	addChild(m_ball);
	PhysicsManager::AddPhysicalObject(*m_ball);

	m_crate = new Crate(glm::vec2(500.0f, 500.0f), glm::vec2(200.0f, 200.0f));
	addChild(m_crate);
	PhysicsManager::AddPhysicalObject(*m_crate);
}

void Scene2::update()
{
	updateDisplayList();
	
}

void Scene2::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_W))
	{
		m_ball->getRigidBody()->velocity.y -= 2.0f;
	}
	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_A))
	{
		m_ball->getRigidBody()->velocity.x -= 2.0f;
	}
	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_D))
	{
		m_ball->getRigidBody()->velocity.x += 2.0f;
	}
	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_S))
	{
		m_ball->getRigidBody()->velocity.y += 2.0f;
	}

	//if (EventManager::Instance().getMouseButton(0))

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}
}

void Scene2::draw()
{
	drawDisplayList();
	Util::DrawQueue();
}

void Scene2::clean()
{
	removeAllChildren();
}