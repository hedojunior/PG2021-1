#include "Sprite.h"

Sprite::Sprite()
{
	initialize();
}

void Sprite::initialize()
{
	float vertices[] = {
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0  // top left 
	};
	
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	
	unsigned int VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	transform = glm::mat4(1);
	texID = -1;
	shader = NULL;
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

void Sprite::setTexture(int texID)
{
	this->texID = texID;
}

void Sprite::setTranslation(glm::vec3 displacements, bool reset)
{
	if (reset) transform = glm::mat4(1);
	transform = glm::translate(transform, displacements);
}

void Sprite::setScale(glm::vec3 scaleFactors, bool reset)
{
	if (reset) transform = glm::mat4(1);
	transform = glm::scale(transform, scaleFactors);
	scale = scaleFactors;
}

void Sprite::draw()
{
	glBindTexture(GL_TEXTURE_2D, texID);
	
	glUniform1i(glGetUniformLocation(shader->ID, "ourTexture1"), 0);

	shader->attachUniforms();
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Sprite::update()
{
	shader->Use();

	setTranslation(pos);
	setScale(scale, false);

	GLint transformLoc = glGetUniformLocation(shader->ID, "model");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
}

bool Sprite::clickInBounds(glm::vec2 clickCoordinates)
{
	float convertedY = 700 - clickCoordinates.y;
	float halfWidth = scale.x / 2;
	float halfHeight = scale.y / 2;

	float minX = pos.x - halfWidth;
	float maxX = pos.x + halfWidth;
	float minY = pos.y - halfHeight;
	float maxY = pos.y + halfHeight;

	bool collidesX = clickCoordinates.x > minX && clickCoordinates.x < maxX;
	bool collidesY = convertedY > minY && convertedY < maxY;

	return collidesX && collidesY;
}
