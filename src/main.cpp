#include "glew.h"
#include "freeglut.h"
#include "glm.hpp"
#include "ext.hpp"
#include <iostream>
#include <cmath>

#include "Shader_Loader.h"
#include "Render_Utils.h"
#include "Camera.h"
#include "Texture.h"


GLuint programColor;
GLuint programTexture;
GLuint programBackground;
GLuint textureChicken;
GLuint textureEarth;
GLuint textureSun;
GLuint textureStars;
GLuint textureJupiter;
GLuint textureNeptun;
GLuint textureShip;
GLuint textureSunOpacity;
GLuint textureEgg;


Core::Shader_Loader shaderLoader;

obj::Model shipModel;
obj::Model sphereModel;
obj::Model chicken;
obj::Model circle;
obj::Model egg;

float cameraAngle = 0;
glm::vec3 cameraPos = glm::vec3(20, 1, 0);
glm::vec3 cameraDir;

glm::mat4 cameraMatrix, perspectiveMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(15.0f, 0.0f, -1.0f));

glm::vec3 lightDir2 = glm::normalize(glm::vec3(0.0f, 15.0f, -1.0f));


void keyboard(unsigned char key, int x, int y)
{
	float angleSpeed = 0.2f;
	float moveSpeed = 0.5f;
	switch(key)
	{
	case 'z': cameraAngle -= angleSpeed; break;
	case 'x': cameraAngle += angleSpeed; break;
	case 'w': cameraPos += cameraDir * moveSpeed; break;
	case 's': cameraPos -= cameraDir * moveSpeed; break;
	case 'd': cameraPos += glm::cross(cameraDir, glm::vec3(0,1,0)) * moveSpeed; break;
	case 'a': cameraPos -= glm::cross(cameraDir, glm::vec3(0,1,0)) * moveSpeed; break;
	case 'q': cameraPos.y += moveSpeed; break;
	case 'e': cameraPos.y -= moveSpeed; break;
	}
}

glm::mat4 createCameraMatrix()
{
	// Obliczanie kierunku patrzenia kamery (w plaszczyznie x-z) przy uzyciu zmiennej cameraAngle kontrolowanej przez klawisze.
	cameraDir = glm::vec3(cosf(cameraAngle), 0.0f, sinf(cameraAngle));
	glm::vec3 up = glm::vec3(0,1,0);

	return Core::createViewMatrix(cameraPos, cameraDir, up);
}

void drawObjectColor(obj::Model * model, glm::mat4 modelMatrix, glm::vec3 color)
{
	GLuint program = programColor;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "objectColor"), color.x, color.y, color.z);
	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);

	Core::DrawModel(model);

	glUseProgram(0);
}

void drawObjectTexture(obj::Model * model, glm::mat4 modelMatrix, GLuint IDtexture)
{
	GLuint program = programTexture;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);
	

	Core::SetActiveTexture(IDtexture, "samp", program, 0);

	Core::DrawModel(model);
	

	glUseProgram(0);
}

void drawBackgroundTexture(obj::Model * model, glm::mat4 modelMatrix, GLuint IDtexture)
{
	GLuint program = programBackground;
	
	glUseProgram(program);



	//glUniform3f(glGetUniformLocation(program, "objectColor"), IDtexture.x, IDtexture.y, IDtexture.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);


	Core::SetActiveTexture(IDtexture, "samp", program, 0);

	Core::DrawModel(model);


	glUseProgram(0);
}

void drawObjectProceduralTexture(obj::Model * model, glm::mat4 modelMatrix, GLuint IDtexture) {
	GLuint program = programTexture;

	glUseProgram(program);

	glUniform3f(glGetUniformLocation(program, "lightDir"), lightDir.x, lightDir.y, lightDir.z);

	glm::mat4 transformation = perspectiveMatrix * cameraMatrix * modelMatrix;
	glUniformMatrix4fv(glGetUniformLocation(program, "modelViewProjectionMatrix"), 1, GL_FALSE, (float*)&transformation);
	glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, (float*)&modelMatrix);


	Core::SetActiveTexture(IDtexture, "samp", program, 0);

	Core::DrawModel(model);


	glUseProgram(0);
}


void renderScene()
{
	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	cameraMatrix = createCameraMatrix();
	perspectiveMatrix = Core::createPerspectiveMatrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);



	glm::mat4 shipModelMatrix = glm::translate(cameraPos + cameraDir * 2.0f + glm::vec3(0,-2.0f,0)) * glm::rotate(-cameraAngle + glm::radians(0.0f), glm::vec3(0,1,0)) * glm::scale(glm::vec3(0.25f));
	
	drawObjectTexture(&shipModel, shipModelMatrix, textureShip);


	//Kurczok

	drawObjectTexture(&chicken, glm::translate(glm::vec3(0, 0, 0)) * glm::rotate(glm::radians(45.0f * time), glm::vec3(1, 12, -1)) * scale(glm::vec3(0.5f)) * glm::translate(glm::vec3(0,0,200)) *  glm::rotate(glm::radians(45.0f * time), glm::vec3(0, 12, 0)), textureChicken);



	drawObjectTexture(&chicken, glm::translate(glm::vec3(0, 0, 0)) * glm::rotate(glm::radians(45.0f * time), glm::vec3(0, 12, 0)) * scale(glm::vec3(0.5f)) * glm::translate(glm::vec3(0, 0, 200)) *  glm::rotate(glm::radians(45.0f * time), glm::vec3(0, 12, 0)), textureChicken);\
	//Moon's kurczak
	drawObjectTexture(&egg, glm::translate(glm::vec3(0,0,200)) * glm::rotate(glm::radians(45.0f * time), glm::vec3(0, 12, 0)), textureEgg);
	
	//Gwiazdy
	drawBackgroundTexture(&sphereModel, glm::translate(glm::vec3(0, 0, 0)) * glm::scale(glm::vec3(60.0f)), textureStars);


	//drawBackgroundTexture(&sphereModel, glm::translate(glm::vec3(0, 0, 0)) * glm::rotate(glm::radians(35.0f * time), glm::vec3(1, 12, -1)) * glm::scale(glm::vec3(5.4f)), textureSunOpacity);
	float planetSizeScale = 0.000005f;
	float angle1 = time * 3.1419f;


	//Ziemia
	glm::mat4 sphereMatrix = glm::translate(glm::vec3(0, 0, 0)) * glm::rotate(glm::radians(45.0f * time), glm::vec3(0, 10, 0));
	sphereMatrix = sphereMatrix * glm::translate(glm::vec3(15, 0, 0)) * glm::rotate(glm::radians(45.0f * time), glm::vec3(0, 12, -1)) * glm::scale(glm::vec3(1.0f));
	drawObjectTexture(&sphereModel, sphereMatrix, textureEarth);


	//Orbity
	drawObjectColor(&circle, glm::translate(glm::vec3(0.0f,0.0f,0.0f)) * glm::scale(glm::vec3(1.5f, 0.02f, 1.5f)), glm::vec3(0.0f, 0.0f, 0.7f));

	//Jowisz
	glm::mat4 sphereMatrixJupiter = glm::translate(glm::vec3(0, 0, 0)) * glm::rotate(glm::radians(35.0f * time), glm::vec3(0, 12, 0));
	sphereMatrixJupiter = sphereMatrixJupiter * glm::translate(glm::vec3(35, 0, 0)) * glm::rotate(glm::radians(75.0f * time), glm::vec3(0.0, 35.0, 0.0)) * glm::scale(glm::vec3(2.15f));

	drawObjectTexture(&sphereModel, sphereMatrixJupiter, textureJupiter);

	//Neptun
	glm::mat4 sphereMatrixNeptun = glm::translate(glm::vec3(0, 0, 0)) * glm::rotate(glm::radians(25.0f * time), glm::vec3(0, 12, 0));
	sphereMatrixNeptun = sphereMatrixNeptun * glm::translate(glm::vec3(55, 0, 0)) * glm::rotate(glm::radians(55.0f * time), glm::vec3(1, 12, -1)) * glm::scale(glm::vec3(1.3f));

	drawObjectTexture(&sphereModel, sphereMatrixNeptun, textureNeptun);


	//Slonce
	drawBackgroundTexture(&sphereModel, glm::translate(glm::vec3(0, 0, 0)) * glm::rotate(glm::radians(35.0f * time), glm::vec3(0, 12, 0)) * glm::scale(glm::vec3(5.0f)), textureSun);
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	float zmienna = 0.1f;
	for (int i = 1; i < 10; i++) {

		drawBackgroundTexture(&sphereModel, glm::translate(glm::vec3(0, 0, 0)) * glm::rotate(glm::radians(35.0f * time), glm::vec3(0, 12, 0)) * glm::scale(glm::vec3(5.0f + zmienna)), textureSunOpacity);
		zmienna += 0.1f;

	}
	glDisable(GL_BLEND);

	glutSwapBuffers();
}

void init()
{
	glEnable(GL_DEPTH_TEST);

	programColor = shaderLoader.CreateProgram("shaders/shader_color.vert", "shaders/shader_color.frag");
	programTexture = shaderLoader.CreateProgram("shaders/shader_tex.vert", "shaders/shader_tex.frag");
	programBackground = shaderLoader.CreateProgram("shaders/shader_tex_background.vert", "shaders/shader_tex_background.frag");
	sphereModel = obj::loadModelFromFile("models/sphere.obj");
	shipModel = obj::loadModelFromFile("models/spaceShip2.obj");
	chicken = obj::loadModelFromFile("models/Chicken.obj");
	circle = obj::loadModelFromFile("models/circle.obj");
	egg = obj::loadModelFromFile("models/Chicken.obj");
	textureSun = Core::LoadTexture("textures/sun.png"); //sun
	textureEarth = Core::LoadTexture("textures/earth.png"); //earth
	textureStars = Core::LoadTexture("textures/stars.png"); //stars
	textureJupiter = Core::LoadTexture("textures/jupiter.png"); //jupiter
	textureNeptun = Core::LoadTexture("textures/neptune.png"); //neptune
	textureChicken = Core::LoadTexture("textures/chicken.png");
	textureSunOpacity = Core::LoadTexture("textures/sunOpacity.png");
	textureShip = Core::LoadTexture("textures/spaceShip2.png");
	textureEgg = Core::LoadTexture("textures/egg.png");


}

void shutdown()
{
	shaderLoader.DeleteProgram(programColor);
	shaderLoader.DeleteProgram(programTexture);
}

void idle()
{
	glutPostRedisplay();
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(200, 200);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Projekt GRK Nikodema i Rafala");
	glewInit();

	init();
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	glutMainLoop();

	shutdown();

	return 0;
}
