#pragma once
#ifndef __BALL_H__
#define __BALL_H__

#include "Sprite.h"

enum BouncingShape { BALL, SQUARE, TRIANGLE };

class BouncingBall final : public Sprite
{
public:
	BouncingBall(glm::vec2 pos);
	~BouncingBall();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

private:
	BouncingShape m_shape;
	
};

#endif /* defined (__PLANE__) */
