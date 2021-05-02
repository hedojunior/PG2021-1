#pragma once

#include "Sprite.h"

class Coin : public Sprite
{
public:
	Coin() : Sprite()
	{
		isFalling = false;
	}

	bool isFalling;
	
	bool collidesWith(Sprite* otherSprite);
};