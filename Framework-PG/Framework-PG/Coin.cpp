#pragma once
#include "Coin.h"

void Coin::startFalling(float speedFactor)
{
	isFalling = true;
	downwardsTranslationY = COIN_MOVEMENT_FACTOR * speedFactor;
	setPosition(glm::vec3(pos.x, pos.y + downwardsTranslationY, 0.0f));
	this->beginAnimating();
}

void Coin::update()
{
	if (isFalling)
	{
		setPosition(glm::vec3(pos.x, pos.y + downwardsTranslationY, 0.0f));
	}

	Sprite::update();
}