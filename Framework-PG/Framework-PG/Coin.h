#pragma once

#include "Sprite.h"

static const float COIN_MOVEMENT_FACTOR = -10.0f;

class Coin : public Sprite
{
public:
	Coin() : Sprite()
	{
		isFalling = false;
	}

	bool isFalling;
	float downwardsTranslationY;
	
	void startFalling(float speedFactor);
	void update() override;
};