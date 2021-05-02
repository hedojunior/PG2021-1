#pragma once

#include "Sprite.h"

static const float COIN_MOVEMENT_FACTOR = -10.0f;
static const float DISAPPEARANCE_Y_LEVEL = -50.0f;

class Coin : public Sprite
{
public:
	Coin() : Sprite()
	{
		isFalling = false;
	}

	bool isFalling;
	bool isCollected;
	float downwardsTranslationY;
	
	void startFalling(float speedFactor);
	void update() override;
};