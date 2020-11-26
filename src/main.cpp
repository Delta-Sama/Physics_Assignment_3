// Core Libraries
#include <crtdbg.h>
#include <iostream>
#include <Windows.h>

#include "Game.h"
#include "Config.h"

int main(int argc, char * args[])
{
	Uint32 frameStart, frameTime;
	UINT32 frames = 0;


	// show console
	AllocConsole();
	freopen("CON", "w", stdout);


	TheGame::Instance()->init("SDLEngine 0.21", 100, 100, 1000, 600, false);

	while (TheGame::Instance()->isRunning())
	{
		frameStart = SDL_GetTicks();

		TheGame::Instance()->handleEvents();
		TheGame::Instance()->update();
		TheGame::Instance()->render();

		frameTime = SDL_GetTicks() - frameStart;

		const float DELAY_TIME = 1000.0f / (float)Config::FPS;
		if (frameTime < DELAY_TIME)
		{
			SDL_Delay(int(DELAY_TIME - frameTime));
		}

		frames++;
		TheGame::Instance()->setFrames(frames);

	}

	TheGame::Instance()->clean();
	return 0;
}

