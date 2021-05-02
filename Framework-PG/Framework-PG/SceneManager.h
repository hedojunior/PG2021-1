#pragma once

#include "Shader.h"
#include "Sprite.h" 
#include "Timer.h"
#include "Coin.h"
#include "SpriteFactory.h"
#include "DeeJay.h"

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
	
	//GLFW callbacks - PRECISAM SER ESTÁTICAS
	//Para isso, as variáveis que modificamos dentro deles
	//também precisam ser e estão no início do SceneManager.cpp
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void resize(GLFWwindow* window, int width, int height);

	//Métodos pricipais
	void initialize(GLuint width, GLuint height);
	void run();
	void finish();

	//Métodos chamados no run (ciclo)
	void update();
	void render();

	void calcWaitingTime(int fps, int elapsedTime)
	{
		waitingTime = 1000 / fps - elapsedTime;
	};

	//Métodos de animação
	void moveRight();
	void moveLeft();
	void dropCoin();

	//Métodos para configuração e carregamento dos elementos da cena
	void initializeGraphics();
	void addShader(string vFilename, string fFilename);
	void setupScene(); //antigo setupGeometry
	void setupCamera2D();

	void displayMessageBox();

private:
	
	//Janela GLFW (por enquanto, assumimos apenas 1 instância)
	GLFWwindow *window;

	//Programa de shader (por enquanto, assumimos apenas 1 instância)
	Shader *shader;

	//Controle de FPS
	Timer *timer;
	double waitingTime;

	//Factory para Sprites
	SpriteFactory* factory;

	//Timer para controlar spawn de moedas
	Timer *gameTimer;

	//Musica
	DeeJay *dj;
	
	//Variáveis de controle para moedas
	double coinSpawnInterval;
	double lastSpawnInSeconds;
	double coinFallingSpeedFactor;
	int collectedCoins;
	int fallenCoins;

	glm::vec4 ortho2D;
	glm::mat4 projection;

	//Sprites
	Sprite* scottPilgrim;

	vector <Sprite*> objects;
	vector <Coin*> coins;

};

