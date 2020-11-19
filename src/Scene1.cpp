#include "Scene1.h"
#include "Game.h"
#include "EventManager.h"

Scene1::Scene1()
{
	Scene1::start();
}

Scene1::~Scene1()
= default;

void Scene1::draw()
{
	drawDisplayList();
	
}

void Scene1::update()
{
	updateDisplayList();

	
}

void Scene1::clean()
{
	removeAllChildren();
	
}

void Scene1::handleEvents()
{
	EventManager::Instance().update();

	//if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
	
	//if (EventManager::Instance().getMouseButton(0))
	
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}
}

void Scene1::start()
{

	
}
