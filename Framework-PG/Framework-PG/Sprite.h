#pragma once

#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sprite
{
public:
	Sprite();
	~Sprite() {}

	void initialize();

	void setTexture(int texID);
	void setShader(Shader* shader) { this->shader = shader; }
	void setPosition(glm::vec3 pos) { this->pos = pos; }
	void setDimension(glm::vec3 scale) { this->scale = scale; }
	void useShader() { shader->Use(); }
	
	void setTranslation(glm::vec3 displacements, bool reset = true);
	void setScale(glm::vec3 scaleFactors, bool reset = true);
	
	void draw();
	void update();

	bool clickInBounds(glm::vec2 clickCoordinates);
	
	unsigned int texID;
	Shader* shader;
protected:
	GLuint VAO;

	glm::mat4 transform;

	glm::vec3 pos, scale;
};

