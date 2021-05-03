#pragma once

#include "Shader.h"
#include "Sprite.h" 
#include "Timer.h"
#include "SpriteFactory.h"
#include "DeeJay.h"
#include "CoinManager.h"

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
	void render();

	//M�todo para c�lculo de FPS
	void calcWaitingTime(int fps, int elapsedTime)
	{
		waitingTime = 1000 / fps - elapsedTime;
	};

	//M�todos de anima��o
	void moveRight();
	void moveLeft();

	//M�todos para configura��o e carregamento dos elementos da cena
	void initializeGraphics();
	void addShader(string vFilename, string fFilename);
	void setupScene();
	void setupCamera2D();

	// M�todo de feedback
	void displayMessageBox();

private:
	//Janela GLFW
	GLFWwindow *window;

	//Programa de shader
	Shader *shader;

	//Controle de FPS
	Timer *timer;
	double waitingTime;

	//Factory para Sprites
	SpriteFactory *factory;

	//Timer para controlar spawn de moedas
	Timer *gameTimer;

	//Musica
	DeeJay *dj;

	//Controlador de moedas
	CoinManager *coinManager;

	//Ortho e proje��o
	glm::vec4 ortho2D;
	glm::mat4 projection;

	//Sprites
	Sprite* scottPilgrim;
	vector <Sprite*> objects;
};

