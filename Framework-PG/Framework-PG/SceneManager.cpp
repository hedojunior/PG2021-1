#include "SceneManager.h"
#include <Windows.h>

//static controllers for mouse and keyboard
static bool keys[1024];
static bool resized;
static GLuint width, height;

//Propriedades constantes
static int const SCOTT_FRAME_COUNT = 8;
static int const SCOTT_ANIM_COUNT = 2;
static int const SCOTT_MOVE_LEFT = 0;
static int const SCOTT_MOVE_RIGHT = 1;
static float const LEFT_BOUNDARY = 30.0f;
static float const RIGHT_BOUNDARY = 570.0f;
static float const FLOOR_LEVEL = 150.0f;

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


	//AQUI aplicaremos as transformações nos sprites
}

void SceneManager::render()
{
	// Clear the colorbuffer
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (resized) //se houve redimensionamento na janela, redefine a projection matrix
	{
		setupCamera2D();
		resized = false;
	}

	//atualiza e desenha os Sprites

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->update();
		objects[i]->draw();
	}
	

}

void SceneManager::calcWaitingTime(int fps, int elapsedTime)
{
	waitingTime = 1000 / fps - elapsedTime;
}

void SceneManager::run()
{
	//GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		timer->start();

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Update method(s)
		update();

		//Render scene
		render();
		
		// Swap the screen buffers
		glfwSwapBuffers(window);
		timer->finish();

		calcWaitingTime(16, timer->getEllapsedTimeMs());

		if (waitingTime)
		{
			Sleep(waitingTime);
		}
	}
}

void SceneManager::finish()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}


void SceneManager::setupScene()
{
	//Criação dos Sprites iniciais -- pode-se fazer métodos de criação posteriormente
	
	//Mínimo: posicao e escala e ponteiro para o shader
	Sprite* background = new Sprite;
	background->setPosition(glm::vec3(400.0f, 300.0f, 0.0f));
	background->setDimension(glm::vec3(800.0f, 600.0f, 1.0f));
	background->setShader(shader);
	objects.push_back(background);

	Sprite* scott = new Sprite;
	scott->setPosition(glm::vec3(LEFT_BOUNDARY, FLOOR_LEVEL, 0.0));
	scott->setDimension(glm::vec3(108.0f, 140.0f, 1.0f)); //note que depois podemos reescalar conforme tamanho da sprite
	scott->setShader(shader);
	objects.push_back(scott);

	//Carregamento das texturas (pode ser feito intercalado na criação)
	//Futuramente, utilizar classe de materiais e armazenar dimensoes, etc
	unsigned int texID = loadTexture("../textures/background.jpg");
	objects[0]->setTexture(texID);
	
	texID = loadTexture("../textures/scott_pilgrim.png");
	objects[1]->setTexture(texID);
	objects[1]->setSpritesheet(SCOTT_ANIM_COUNT, SCOTT_FRAME_COUNT, SCOTT_MOVE_RIGHT);
	objects[1]->updateVAO();

	//Definindo a janela do mundo (ortho2D)
	ortho2D[0] = 0.0f; //xMin
	ortho2D[1] = 800.0f; //xMax
	ortho2D[2] = 0.0f; //yMin
	ortho2D[3] = 600.0f; //yMax

	//Habilita transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SceneManager::setupCamera2D()
{
	float zNear = -1.0, zFar = 1.0; //estão fixos porque não precisamos mudar

	projection = glm::ortho(ortho2D[0], ortho2D[1], ortho2D[2], ortho2D[3], zNear, zFar);


	//Obtendo o identificador da matriz de projeção para enviar para o shader
	GLint projLoc = glGetUniformLocation(shader->ID, "projection");
	//Enviando a matriz de projeção para o shader
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

unsigned int SceneManager::loadTexture(string filename)
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
	
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	
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
