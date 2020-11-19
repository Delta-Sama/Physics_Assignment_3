#pragma once
#ifndef __CONFIG__
#define __CONFIG__

#include <SDL_ttf.h>
#include <memory>
#include "glm/vec2.hpp"

class Config {
public:
	static const int SCREEN_WIDTH = 1000;
	static const int SCREEN_HEIGHT = 600;
	static const int FPS = 60;

	static constexpr float g = -9.8f;
	static constexpr float BULLET_SPEED = 3.0f;
	
	//static constexpr float MET_TO_PIX = (SCREEN_WIDTH - START_POINT) / MAX_DIST;
	//static constexpr float PIX_TO_MET = 1 / MET_TO_PIX;
	
	// Define Custom Deleters for shared_ptr types
	static void SDL_DelRes(SDL_Window* r) { SDL_DestroyWindow(r); }
	static void SDL_DelRes(SDL_Renderer* r) { SDL_DestroyRenderer(r); }
	static void SDL_DelRes(SDL_Texture* r) { SDL_DestroyTexture(r); }
	static void SDL_DelRes(SDL_Surface* r) { SDL_FreeSurface(r); }
	static void SDL_DelRes(TTF_Font* r) { TTF_CloseFont(r); }

	// template function to create and return shared_ptr instance
	template <typename T>
	static std::shared_ptr<T> make_resource(T* t) {
		return std::shared_ptr<T>(t, [](T* t) { Config::SDL_DelRes(t); });
	}

};

#endif /* defined (__CONFIG__) */