#include "SpriteFactory.h"

#include "Sprite.h"
#include "Shader.h"

SpriteFactory::SpriteFactory()
{

}

SpriteFactory::~SpriteFactory()
{

}

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

Sprite* SpriteFactory::sky(Shader* shader)
{
	return commonLayer("../textures/sky.png", shader);
}

Sprite* SpriteFactory::background(Shader* shader)
{
	return commonLayer("../textures/back.png", shader);
}

Sprite* SpriteFactory::backHouses(Shader* shader)
{
	return commonLayer("../textures/houses3.png", shader);
}

Sprite* SpriteFactory::frontHouses(Shader* shader)
{
	return commonLayer("../textures/houses1.png", shader);
}

Sprite* SpriteFactory::minorBuildings(Shader* shader)
{
	return commonLayer("../textures/minishop&callbox.png", shader);
}

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

Sprite* SpriteFactory::streetAndLamps(Shader* shader)
{
	return commonLayer("../textures/road&lamps.png", shader);
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