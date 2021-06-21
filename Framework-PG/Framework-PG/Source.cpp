/* Hello Transforms - c�digo inicial fornecido em https://learnopengl.com/#!Getting-started/Hello-Triangle 
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Computa��o Gr�fica - Jogos Digitais - Unisinos
 * Vers�o inicial: 7/4/2017
 * �ltima atualiza��o em 14/03/2018
 *
 */

using namespace std;

#include "SceneManager.h"
#include <string.h>


// The MAIN function, from here we start the application and run the game loop
int main(int argc, char** argv)
{
	char imagePath[255];

	printf("Please, provide the image path:\n");

	scanf_s("%[^\n]254s%*", imagePath, 254);

	SceneManager *scene = new SceneManager;
	scene->initialize(1200, 700, imagePath);
	
	scene->run();

	scene->finish();
	
	return 0;
}


