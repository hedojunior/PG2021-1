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

	~SpriteFactory() {

	}

	Sprite* sky(Shader* shader);
	Sprite* background(Shader* shader);
	Sprite* backHouses(Shader* shader);
	Sprite* frontHouses(Shader* shader);
	Sprite* minorBuildings(Shader* shader);
	Sprite* scottPilgrim(Shader* shader, float initialX, float initialY, int initialAnimIndex);
	Sprite* streetAndLamps(Shader* shader);

	Coin* coin(Shader* shader);

private:
	Sprite* commonLayer(string textureFile, Shader* shader);
	unsigned int loadTexture(string filename);
};