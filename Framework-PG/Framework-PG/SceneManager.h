#pragma once

#include "Shader.h"
#include "Sprite.h" 
#include "SpriteFactory.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace std;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	
	//GLFW callbacks
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void resize(GLFWwindow* window, int width, int height);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	//Métodos pricipais
	void initialize(GLuint width, GLuint height, string imagePath);
	void run();
	void finish();

	//Métodos chamados no run (ciclo)
	void update();
	void render();

	//Métodos para configuração e carregamento dos elementos da cena
	void initializeGraphics();
	void setupShaders();
	void setupStickers();
	void setupFilterPreviews();
	void setupScene();
	void setupCamera2D();

	int loadTexture(string filename);

	void handleClick();

private:
	
	GLFWwindow *window;
	SpriteFactory* spriteFactory;

	glm::vec4 ortho2D;
	glm::mat4 projection;

	Sprite* mainImage;
	string imagePath;
	
	vector <Sprite*> filters;
	vector <Sprite*> stickers;
	vector <Sprite*> placedStickers;
	
	Sprite* selectedFilter;
	Sprite* selectedSticker;

	Shader* textureShader;
	vector <Shader*> filterShaders;

};

