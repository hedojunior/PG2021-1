#pragma once

#include "Sprite.h"
#include "Shader.h"

using namespace std;

class SpriteFactory
{
public:
	SpriteFactory() {

	}

	Sprite* sticker(string textureFile, Shader* shader, glm::vec3 position);
	Sprite* sticker(int textureID, Shader* shader, glm::vec3 position);

private:
	unsigned int loadTexture(string filename);
};