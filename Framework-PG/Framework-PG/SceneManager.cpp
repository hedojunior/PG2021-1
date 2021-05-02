#include "SceneManager.h"
#include <Windows.h>

//static controllers for mouse and keyboard
static bool keys[1024];
static bool resized;
static GLuint width, height;

//Propriedades constantes
static int const SCOTT_MOVE_LEFT = 0;
static int const SCOTT_MOVE_RIGHT = 1;
static int const FPS = 14;

static float const LEFT_BOUNDARY = 60.0f;
static float const RIGHT_BOUNDARY = 750.0f;
static float const FLOOR_LEVEL = 160.0f;

static float const MOVEMENT_FACTOR = 25.0f;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint w, GLuint h)
{
	width = w;
	height = h;
	timer = new Timer;
	waitingTime = 0;
	factory = new SpriteFactory;
	gameTimer = new Timer;
	coinSpawnInterval = 3;
	lastSpawnInSeconds = 0;
	coinFallingSpeedFactor = 1.0;

	gameTimer->start();
	
	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();

}

void SceneManager::initializeGraphics()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "Hello Sprites", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	//Setando a callback de redimensionamento da janela
	glfwSetWindowSizeCallback(window, resize);
	
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Build and compile our shader program
	addShader("../shaders/transformations.vs", "../shaders/transformations.frag");

	//setup the scene -- LEMBRANDO QUE A DESCRIÇÃO DE UMA CENA PODE VIR DE ARQUIVO(S) DE 
	// CONFIGURAÇÃO
	setupScene();

	resized = true; //para entrar no setup da câmera na 1a vez

}

void SceneManager::addShader(string vFilename, string fFilename)
{
	shader = new Shader (vFilename.c_str(), fFilename.c_str());
}


void SceneManager::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void SceneManager::resize(GLFWwindow * window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	// Define the viewport dimensions
	glViewport(0, 0, width, height);
}


void SceneManager::update()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) 
	{
		moveRight();
	}
	else if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		moveLeft();
	}
	else
	{
		scottPilgrim->stopAnimating();
	}


	if (gameTimer->getTimeInSeconds() - lastSpawnInSeconds >= coinSpawnInterval)
	{
		//lastSpawnInSeconds = gameTimer->getTimeInSeconds();
		//cout << lastSpawnInSeconds;
		//drop coin
	}
		
}

void SceneManager::render()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (resized)
	{
		setupCamera2D();
		resized = false;
	}

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->update();
		objects[i]->draw();
	}

	for (int i = 0; i < coins.size(); i++)
	{
		coins[i]->update();
		coins[i]->draw();
	}
}

void SceneManager::moveRight()
{
	scottPilgrim->setAnimationIndex(SCOTT_MOVE_RIGHT);

	if (scottPilgrim->canMoveRight(RIGHT_BOUNDARY))
	{
		scottPilgrim->beginAnimating();
		scottPilgrim->updateXAxisPosition(MOVEMENT_FACTOR);
	}
}

void SceneManager::moveLeft()
{
	scottPilgrim->setAnimationIndex(SCOTT_MOVE_LEFT);

	if (scottPilgrim->canMoveLeft(LEFT_BOUNDARY))
	{
		scottPilgrim->beginAnimating();
		scottPilgrim->updateXAxisPosition(-MOVEMENT_FACTOR);
	}
}

void SceneManager::run()
{
	//GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		timer->start();

		glfwPollEvents();

		update();

		render();
		
		glfwSwapBuffers(window);

		timer->finish();

		calcWaitingTime(FPS, timer->getEllapsedTimeMs());

		if (waitingTime)
		{
			Sleep(waitingTime);
		}
	}
}

void SceneManager::finish()
{
	glfwTerminate();
}


void SceneManager::setupScene()
{
	objects.push_back(factory->sky(shader));
	objects.push_back(factory->background(shader));
	objects.push_back(factory->backHouses(shader));
	objects.push_back(factory->frontHouses(shader));
	objects.push_back(factory->minorBuildings(shader));
	objects.push_back(factory->streetAndLamps(shader));

	scottPilgrim = factory->scottPilgrim(shader, LEFT_BOUNDARY, FLOOR_LEVEL, SCOTT_MOVE_RIGHT);
	objects.push_back(scottPilgrim);

	for (int i = 0; i <= 50; i++)
	{
		Coin* coin = factory->coin(shader);
		coins.push_back(coin);
	}

	ortho2D[0] = 0.0f;
	ortho2D[1] = 800.0f;
	ortho2D[2] = 0.0f;
	ortho2D[3] = 600.0f;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SceneManager::setupCamera2D()
{
	float zNear = -1.0, zFar = 1.0;

	projection = glm::ortho(ortho2D[0], ortho2D[1], ortho2D[2], ortho2D[3], zNear, zFar);

	GLint projLoc = glGetUniformLocation(shader->ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}
