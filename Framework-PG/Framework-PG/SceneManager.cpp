#include "SceneManager.h"

static bool keys[1024];
static bool resized;
static GLuint width, height;

static glm::vec2 clickCoordinates;
static bool clicked;

static string STICKER_BE_MINE = "../textures/be-mine.png";
static string STICKER_COFFEE_CUP = "../textures/coffee-cup.png";
static string STICKER_DOUGHNUT = "../textures/doughnut.png";
static string STICKER_FLASH = "../textures/flash.png";
static string STICKER_HEART = "../textures/heart.png";
static string STICKER_PARTY = "../textures/party.png";
static string STICKER_PRIDE = "../textures/pride.png";
static string STICKER_STAR = "../textures/star.png";

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint w, GLuint h, char path[255])
{
	width = w;
	height = h;

	for (int i = 0; i < 255; i++)
	{
		imagePath[i] = path[i];
	}

	spriteFactory = new SpriteFactory();
	
	initializeGraphics();

}

void SceneManager::initializeGraphics()
{
	glfwInit();

	window = glfwCreateWindow(width, height, "Fakestagram", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	glfwSetWindowSizeCallback(window, resize);

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	setupShaders();

	setupScene();

	resized = true;

}

void SceneManager::setupShaders()
{
	textureShader = new Shader("../shaders/transformations.vs", "../shaders/texture.frag");

	Shader * shader = new Shader("../shaders/transformations.vs", "../shaders/binarized.frag");
	shader->setUniform("binarizationFactor", 0.7);
	filterShaders.push_back(shader);

	shader = new Shader("../shaders/transformations.vs", "../shaders/inverted.frag");
	filterShaders.push_back(shader);

	shader = new Shader("../shaders/transformations.vs", "../shaders/grayscaled.frag");
	filterShaders.push_back(shader);

	shader = new Shader("../shaders/transformations.vs", "../shaders/colored.frag");
	shader->setUniform("rgbModifier", glm::vec3(1.0, 0.0, 0.0));
	filterShaders.push_back(shader);

	shader = new Shader("../shaders/transformations.vs", "../shaders/colored.frag");
	shader->setUniform("rgbModifier", glm::vec3(0.0, 0.5, 0.0));
	filterShaders.push_back(shader);

	shader = new Shader("../shaders/transformations.vs", "../shaders/colored.frag");
	shader->setUniform("rgbModifier", glm::vec3(0.0, 0.0, 1.0));
	filterShaders.push_back(shader);

	shader = new Shader("../shaders/transformations.vs", "../shaders/colored.frag");
	shader->setUniform("rgbModifier", glm::vec3(0.48, 0.25, 0.0));
	filterShaders.push_back(shader);

	shader = new Shader("../shaders/transformations.vs", "../shaders/colored.frag");
	shader->setUniform("rgbModifier", glm::vec3(1.0, 0.031, 0.87));
	filterShaders.push_back(shader);
}

void SceneManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		double posX, posY;
		glfwGetCursorPos(window, &posX, &posY);

		clickCoordinates = glm::vec2(posX, posY);
		clicked = true;
	}

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

	glViewport(0, 0, width, height);
}


void SceneManager::update()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (clicked)
	{
		clicked = false;
		handleClick();
	}


}

void SceneManager::render()
{
	glClearColor(0.8f, 0.57f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (resized)
	{
		setupCamera2D();
		resized = false;
	}

	mainImage->update();
	mainImage->draw();

	for (int i = 0; i < stickers.size(); i++)
	{
		stickers[i]->update();
		stickers[i]->draw();
	}
	
	for (int i = 0; i < filters.size(); i++)
	{
		filters[i]->update();
		filters[i]->draw();
	}

	for (int i = 0; i < placedStickers.size(); i++)
	{
		placedStickers[i]->update();
		placedStickers[i]->draw();
	}

}

void SceneManager::run()
{
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		update();

		render();
		
		glfwSwapBuffers(window);
	}
}

void SceneManager::finish()
{
	glfwTerminate();
}

void SceneManager::handleClick()
{
	if (selectedStickerTextureID > 0)
	{
		if (mainImage->clickInBounds(clickCoordinates))
		{
			glm::vec3 position = glm::vec3(clickCoordinates.x, 700 - clickCoordinates.y, 1.0);
			Sprite* newSticker = spriteFactory->sticker(selectedStickerTextureID, textureShader, position);
			placedStickers.push_back(newSticker);
		}

		selectedStickerTextureID = 0;
	}
	else
	{
		for (int i = 0; i < stickers.size(); i++)
		{
			if (stickers[i]->clickInBounds(clickCoordinates))
			{
				selectedStickerTextureID = stickers[i]->texID;
				return;
			}
		}

		for (int i = 0; i < filters.size(); i++)
		{
			if (filters[i]->clickInBounds(clickCoordinates))
			{
				mainImage->setShader(filters[i]->shader);
				return;
			}
		}
	}
}

void SceneManager::setupStickers()
{
	float baseY = 150.0f;
	float baseX = 50.0f;
	float size = 70.0f;

	Sprite* sprite = spriteFactory->sticker(STICKER_BE_MINE, textureShader, glm::vec3(baseX + size, baseY + size, 1.0f));
	stickers.push_back(sprite);

	sprite = spriteFactory->sticker(STICKER_COFFEE_CUP, textureShader, glm::vec3(baseX + size * 2, baseY + size, 1.0f));
	stickers.push_back(sprite);

	sprite = spriteFactory->sticker(STICKER_DOUGHNUT, textureShader, glm::vec3(baseX + size, baseY + size * 2, 1.0f));
	stickers.push_back(sprite);

	sprite = spriteFactory->sticker(STICKER_FLASH, textureShader, glm::vec3(baseX + size * 2, baseY + size * 2, 1.0f));
	stickers.push_back(sprite);

	sprite = spriteFactory->sticker(STICKER_HEART, textureShader, glm::vec3(baseX + size, baseY + size * 3, 1.0f));
	stickers.push_back(sprite);

	sprite = spriteFactory->sticker(STICKER_PARTY, textureShader, glm::vec3(baseX + size * 2, baseY + size * 3, 1.0f));
	stickers.push_back(sprite);

	sprite = spriteFactory->sticker(STICKER_PRIDE, textureShader, glm::vec3(baseX + size, baseY + size * 4, 1.0f));
	stickers.push_back(sprite);

	sprite = spriteFactory->sticker(STICKER_STAR, textureShader, glm::vec3(baseX + size * 2, baseY + size * 4, 1.0f));
	stickers.push_back(sprite);
}

void SceneManager::setupFilterPreviews()
{

	float width = 100.0f;
	float height = 100.0f;
	float firstColumnX = 970.0f;
	float secondColumnX = firstColumnX + width + 20;
	float positionY = 180.0f;

	for (int i = 0; i < filterShaders.size(); i = i + 2)
	{
		Sprite* sprite = new Sprite;
		sprite->setDimension(glm::vec3(width, height, 1.0f));
		sprite->setPosition(glm::vec3(firstColumnX, positionY, 1.0f));
		sprite->setShader(filterShaders[i]);
		sprite->setTexture(loadTexture(imagePath));
		filters.push_back(sprite);

		sprite = new Sprite;
		sprite->setDimension(glm::vec3(width, height, 1.0f));
		sprite->setPosition(glm::vec3(secondColumnX, positionY, 1.0f));
		sprite->setShader(filterShaders[i + 1]);
		sprite->setTexture(loadTexture(imagePath));
		filters.push_back(sprite);

		positionY += height + 10;
	}
	
}


void SceneManager::setupScene()
{

	setupStickers();
	setupFilterPreviews();

	Sprite* sprite = new Sprite;
	sprite->setDimension(glm::vec3(500.0f, 500.0f, 1.0f));
	sprite->setPosition(glm::vec3(600.0f, 350.0f, 1.0f));
	sprite->setShader(textureShader);
	sprite->setTexture(loadTexture(imagePath));
	mainImage = sprite;

	ortho2D[0] = 0.0f; //xMin
	ortho2D[1] = 1200.0f; //xMax
	ortho2D[2] = 0.0f; //yMin
	ortho2D[3] = 700.0f; //yMax

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SceneManager::setupCamera2D()
{
	float zNear = -1.0, zFar = 1.0;

	projection = glm::ortho(ortho2D[0], ortho2D[1], ortho2D[2], ortho2D[3], zNear, zFar);

	for (int i = 0; i < filterShaders.size(); i++)
	{
		filterShaders[i]->Use();
		
		GLint projLoc = glGetUniformLocation(filterShaders[i]->ID, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	}

	textureShader->Use();
	GLint projLoc = glGetUniformLocation(textureShader->ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

int SceneManager::loadTexture(string filename)
{
	unsigned int texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else
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

	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);

	return texture;
}