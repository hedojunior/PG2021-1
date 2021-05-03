#pragma once

#include "Sprite.h"
#include "Shader.h"
#include "Coin.h"

using namespace std;

class SpriteFactory
{
public:
	SpriteFactory() {

	}
	
	// Sprites do layering do fundo (não deu tempo do parallax D:)
	Sprite* sky(Shader* shader);
	Sprite* background(Shader* shader);
	Sprite* backHouses(Shader* shader);
	Sprite* frontHouses(Shader* shader);
	Sprite* minorBuildings(Shader* shader);
	Sprite* streetAndLamps(Shader* shader);

	Sprite* scottPilgrim(Shader* shader, float initialX, float initialY, int initialAnimIndex);

	Coin* coin(Shader* shader);

private:
	//Como as layers de fundo são parecidas (800x600), um método genérico para usar em cada uma delas.
	Sprite* commonLayer(string textureFile, Shader* shader);
	unsigned int loadTexture(string filename);
};