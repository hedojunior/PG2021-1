#include "Sprite.h"

Sprite::Sprite()
{
	initialize();
}

void Sprite::initialize()
{
	nAnims = 1;
	nFrames = 1;
	iFrame = 0;
	iAnim = 0;
	usesSpritesheet = false;
	isAnimating = false;

	updateVAO();

	transform = glm::mat4(1); //matriz identidade
	texID = -1; //ainda não temos
	shader = NULL; //ainda não temos
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	angle = 0.0f;
}

void Sprite::setTexture(int texID)
{
	this->texID = texID;
}

void Sprite::setRotation(float angle, glm::vec3 axis, bool reset)
{
	if (reset) transform = glm::mat4(1);
	transform = glm::rotate(transform, angle, axis);
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

void Sprite::setSpritesheet(int numAnim, int numFrames, int initialAnim)
{
	this->nAnims = numAnim;
	this->nFrames = numFrames;
	this->iAnim = initialAnim;
	this->usesSpritesheet = true;
}

bool Sprite::collidesWith(Sprite* otherSprite)
{
	bool collisionX = this->pos.x + this->scale.x >= otherSprite->pos.x &&
		otherSprite->pos.x + otherSprite->scale.x >= this->pos.x;

	bool collisionY = this->pos.y + this->scale.y >= otherSprite->pos.y &&
		otherSprite->pos.y + otherSprite->scale.y >= this->pos.y;

	return collisionX && collisionY;
}

void Sprite::draw()
{
	glBindTexture(GL_TEXTURE_2D, texID);
	glUniform1i(glGetUniformLocation(shader->ID, "ourTexture1"), 0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Sprite::update()
{
	setTranslation(pos);
	setRotation(angle, glm::vec3(0.0f, 0.0f, 1.0f), false);
	setScale(scale, false);

	GLint transformLoc = glGetUniformLocation(shader->ID, "model");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	GLint offsetXLoc = glGetUniformLocation(shader->ID, "offsetX");
	float offsetX = (usesSpritesheet && isAnimating) ? iFrame * dX : 0;
	glUniform1f(offsetXLoc, offsetX);

	GLint offsetYLoc = glGetUniformLocation(shader->ID, "offsetY");
	float offsetY = usesSpritesheet ? iAnim * dY : 0;
	glUniform1f(offsetYLoc, offsetY);

	if (usesSpritesheet && isAnimating) 
	{
		iFrame = (iFrame + 1) % nFrames;
	}
}

void Sprite::updateVAO()
{
	dX = 1.0 / nFrames;
	dY = 1.0 / nAnims;

	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   dX, dY, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   dX, 0.0, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, dY  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
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
}
