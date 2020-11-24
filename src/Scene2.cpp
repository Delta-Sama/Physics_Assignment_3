#include "Scene2.h"

#include "Game.h"
#include "EventManager.h"

Scene2::Scene2()
{
	Scene2::start();
}

Scene2::~Scene2()
= default;

// SCENE

void Scene2::start()
{
	m_ball = new BouncingBall(glm::vec2(100.0f,100.0f));
	addChild(m_ball);
	
}

void Scene2::update()
{
	updateDisplayList();

	
}

void Scene2::handleEvents()
{
	EventManager::Instance().update();

	//if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))

	//if (EventManager::Instance().getMouseButton(0))

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}
}

void Scene2::draw()
{
	drawDisplayList();
}

void Scene2::clean()
{
	removeAllChildren();
}