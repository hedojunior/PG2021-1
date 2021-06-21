#include "SpriteFactory.h"

Sprite* SpriteFactory::sticker(string textureFile, Shader* shader, glm::vec3 position)
{
	Sprite* layer = new Sprite;
	layer->setPosition(position);
	layer->setDimension(glm::vec3(70.0f, 70.0f, 1.0f));
	layer->setShader(shader);

	unsigned int textureID = loadTexture(textureFile);  
	layer->setTexture(textureID);

	return layer;
}

Sprite* SpriteFactory::sticker(int textureID, Shader* shader, glm::vec3 position)
{
	Sprite* layer = new Sprite;
	layer->setPosition(position);
	layer->setDimension(glm::vec3(70.0f, 70.0f, 1.0f));
	layer->setShader(shader);
	layer->setTexture(textureID);

	return layer;
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