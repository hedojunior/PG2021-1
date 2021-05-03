#pragma once

#include "Sprite.h"

//Velocidade de movimento da moeda
static const float COIN_MOVEMENT_FACTOR = -10.0f;

//Valor mínimo para considerar como moeda não pega
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

	//Variável indicativa do valor de translação em Y (valor de movimento * fator de escala de velocidade)
	float downwardsTranslationY;
	
	void startFalling(float speedFactor);
	void update() override;
};