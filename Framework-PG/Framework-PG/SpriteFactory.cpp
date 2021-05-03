#include "SpriteFactory.h"

Sprite* SpriteFactory::commonLayer(string textureFile, Shader* shader)
{
	Sprite* layer = new Sprite;
	layer->setPosition(glm::vec3(400.0f, 300.0f, 0.0f));
	layer->setDimension(glm::vec3(800.0f, 600.0f, 1.0f));
	layer->setShader(shader);

	unsigned int textureID = loadTexture(textureFile);
	layer->setTexture(textureID);

	return layer;
}

Sprite* SpriteFactory::sky(Shader* shader) { return commonLayer("../textures/sky.png", shader); }

Sprite* SpriteFactory::background(Shader* shader) { return commonLayer("../textures/back.png", shader); }

Sprite* SpriteFactory::backHouses(Shader* shader) { return commonLayer("../textures/houses3.png", shader); }

Sprite* SpriteFactory::frontHouses(Shader* shader) { return commonLayer("../textures/houses1.png", shader); }

Sprite* SpriteFactory::minorBuildings(Shader* shader) { return commonLayer("../textures/minishop&callbox.png", shader); }

Sprite* SpriteFactory::streetAndLamps(Shader* shader) { return commonLayer("../textures/road&lamps.png", shader); }

//Número de frames e animações da spritesheet do Scott.
static int const SCOTT_FRAME_COUNT = 8;
static int const SCOTT_ANIM_COUNT = 2;

Sprite* SpriteFactory::scottPilgrim(Shader* shader, float initialX, float initialY, int initialAnimIndex)
{
	Sprite* scott = new Sprite;
	scott->setPosition(glm::vec3(initialX, initialY, 0.0));
	scott->setDimension(glm::vec3(108.0f, 140.0f, 1.0f));
	scott->setShader(shader);

	unsigned int textureID = loadTexture("../textures/scott_pilgrim.png");
	scott->setTexture(textureID);
	scott->setSpritesheet(SCOTT_ANIM_COUNT, SCOTT_FRAME_COUNT, initialAnimIndex);
	scott->updateVAO();

	return scott;
}

//Número de frames e animações da moeda, bem como o Y inicial (em cima da tela)
static int const COIN_FRAME_COUNT = 4;
static int const COIN_ANIM_COUNT = 1;
static float const COIN_INITIAL_Y = 700;

Coin* SpriteFactory::coin(Shader* shader)
{
	Coin* coin = new Coin;

	//Fator randômico para gerar um valor entre 60 e 750 (os limites definidos da tela) para criar a moeda
	float randomFactor = rand() % (750 - 60 + 1) + 60;
	
	coin->setPosition(glm::vec3(randomFactor, COIN_INITIAL_Y, 0.0));
	coin->setDimension(glm::vec3(40.0f, 44.0f, 1.0f));
	coin->setShader(shader);

	unsigned int textureID = loadTexture("../textures/coin.png");
	coin->setTexture(textureID);
	coin->setSpritesheet(COIN_ANIM_COUNT, COIN_FRAME_COUNT, 0);
	coin->updateVAO();

	return coin;
}

unsigned int SpriteFactory::loadTexture(string filename)
{
	unsigned int texture;

	// load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;

	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glActiveTexture(GL_TEXTURE0);

	return texture;
}