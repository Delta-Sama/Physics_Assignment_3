#pragma once
#ifndef __GAME_OBJECT_TYPE__
#define __GAME_OBJECT_TYPE__
enum GameObjectType {
	NONE = -1,
	PLAYER,
	BULLET,
	START_BUTTON,
	RESTART_BUTTON,
	BACK_BUTTON,
	NEXT_BUTTON,
	BACKGROUND,
	NUM_OF_TYPES,
	ENEMY
};
#endif /* defined (__GAME_OBJECT_TYPE__) */