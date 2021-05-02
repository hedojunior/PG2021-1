#pragma once
#include "Coin.h"

void Coin::startFalling(float speedFactor)
{
	isCollected = false;
	isFalling = true;
	downwardsTranslationY = COIN_MOVEMENT_FACTOR * speedFactor;
	setPosition(glm::vec3(pos.x, pos.y + downwardsTranslationY, 0.0f));
	this->beginAnimating();
}

void Coin::update()
{
	if (isFalling)
	{
		float nextYPosition = pos.y + downwardsTranslationY;

		if (nextYPosition <= DISAPPEARANCE_Y_LEVEL || isCollected) 
		{
			isFalling = false;
			this->stopAnimating();
			nextYPosition = 1000;
		}

		setPosition(glm::vec3(pos.x, nextYPosition, 0.0f));
	}

	Sprite::update();
}