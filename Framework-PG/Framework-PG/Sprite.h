#pragma once

#include "Shader.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sprite
{
public:
	Sprite();
	~Sprite() {}

	void initialize(); //similar ao setupGeometry

	//Para setar os atributos 
	void setTexture(int texID);
	void setShader(Shader* shader) { this->shader = shader; shader->Use(); }
	void setPosition(glm::vec3 pos) { this->pos = pos; }
	void setDimension(glm::vec3 scale) { this->scale = scale; }
	void setAngle(float angle) { this->angle = angle; } //para 3D precisa angulo por eixo ou quaternion
	void setSpritesheet(int numAnim, int numFrames, int initialAnim);
	void setAnimationIndex(int iAnim) { this->iAnim = iAnim; }

	//Para controle direto da matriz de transformações
	void setRotation(float angle, glm::vec3 axis, bool reset = true);
	void setTranslation(glm::vec3 displacements, bool reset = true);
	void setScale(glm::vec3 scaleFactors, bool reset = true);
	
	//Chamados no ciclo do SceneManager
	void draw();
	virtual void update();

	//métodos para animação
	void updateVAO();

	void beginAnimating() { this->isAnimating = true; }
	void stopAnimating() { this->isAnimating = false; }

	//Indicam se a sprite pode se movimentar baseado nos limites de tela
	bool canMoveRight(float boundary) { return pos.x < boundary; }
	bool canMoveLeft(float boundary) { return pos.x > boundary; }

	//Indica se esse Sprite colide com outro Sprite
	bool collidesWith(Sprite* otherSprite);

	//Atualiza a posição em X adicionando um fator passado por parâmetro
	void updateXAxisPosition(float factor) { this->pos = glm::vec3(this->pos.x + factor, this->pos.y, this->pos.z); }

protected:
	//Atributos gerais

	GLuint VAO; //ID do buffer de geometria

	glm::mat4 transform; //matriz de transformação

	unsigned int texID; //identificador da textura

	Shader* shader; //ponteiro para o objeto de shader, para fazer a interface

	//Atributos de controle da posição, orientação e dimensões do sprite
	glm::vec3 pos, scale;
	float angle;

	//Atributos para controle da animação
	int nAnims, nFrames;
	int iAnim, iFrame; //indices da animação e frame atual

	float dX, dY;

	bool usesSpritesheet, isAnimating;
};

