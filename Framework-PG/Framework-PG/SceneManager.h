#pragma once

#include "Shader.h"
#include "Sprite.h" 
#include "Timer.h"
#include "SpriteFactory.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	
	//GLFW callbacks - PRECISAM SER EST�TICAS
	//Para isso, as vari�veis que modificamos dentro deles
	//tamb�m precisam ser e est�o no in�cio do SceneManager.cpp
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void resize(GLFWwindow* window, int width, int height);

	//M�todos pricipais
	void initialize(GLuint width, GLuint height);
	void run();
	void finish();

	//M�todos chamados no run (ciclo)
	void update();
	void calcWaitingTime(int fps, int elapsedTime);
	void render();

	//M�todos de anima��o
	void moveRight();
	void moveLeft();

	//M�todos para configura��o e carregamento dos elementos da cena
	void initializeGraphics();
	void addShader(string vFilename, string fFilename);
	void setupScene(); //antigo setupGeometry
	void setupCamera2D();

private:
	
	//Janela GLFW (por enquanto, assumimos apenas 1 inst�ncia)
	GLFWwindow *window;

	//Programa de shader (por enquanto, assumimos apenas 1 inst�ncia)
	Shader *shader;

	//Controle de FPS
	Timer *timer;
	double waitingTime;

	//Factory para Sprites
	SpriteFactory* factory;

	//C�mera 2D - Matriz de proje��o (ortogr�fica) com os limites em x,y
	glm::vec4 ortho2D; //xmin, xmax, ymin, ymax
	glm::mat4 projection;

	//Nossos objetos (sprites) da cena
	vector <Sprite*> objects;

};

