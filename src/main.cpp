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



Core::Shader_Loader shaderLoader;

obj::Model shipModel;
obj::Model sphereModel;
obj::Model chicken;

float cameraAngle = 0;
glm::vec3 cameraPos = glm::vec3(-5, 0, 0);
glm::vec3 cameraDir;

glm::mat4 cameraMatrix, perspectiveMatrix;

glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, -0.9f, -1.0f));

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

	glm::mat4 shipModelMatrix = glm::translate(cameraPos + cameraDir * 0.5f + glm::vec3(0,-0.25f,0)) * glm::rotate(-cameraAngle + glm::radians(90.0f), glm::vec3(0,1,0)) * glm::scale(glm::vec3(0.25f));
	
	drawObjectColor(&shipModel, shipModelMatrix, glm::vec3(0.6f));

<<<<<<< HEAD
	//Gwiazdy
=======
	drawObjectTexture(&chicken, glm::translate(glm::vec3(0, 0, 100)) *glm::scale(glm::vec3(0.5f)), textureChicken);

>>>>>>> a1f106886ae3364ff56381078c774025678898ac
	drawBackgroundTexture(&sphereModel, glm::translate(glm::vec3(0, 0, 0)) * glm::scale(glm::vec3(60.0f)), textureStars);

	//Slonce
	drawBackgroundTexture(&sphereModel, glm::translate(glm::vec3(0, 0, 0)) * glm::rotate(glm::radians(35.0f * time), glm::vec3(1, 12, -1)) * glm::scale(glm::vec3(5.0f)), textureSun);

	//Ziemia
	glm::mat4 sphereMatrix = glm::translate(glm::vec3(0, 0, 0)) * glm::rotate(glm::radians(45.0f * time), glm::vec3(1, 12, -1));
	sphereMatrix = sphereMatrix * glm::translate(glm::vec3(15, 0, 0)) * glm::rotate(glm::radians(45.0f * time), glm::vec3(1, 12, -1)) * glm::scale(glm::vec3(1.0f));
	drawObjectTexture(&sphereModel, sphereMatrix, textureEarth);

	//Jowisz
	glm::mat4 sphereMatrixJupiter = glm::translate(glm::vec3(0, 0, 0)) * glm::rotate(glm::radians(35.0f * time), glm::vec3(1, 12, -1));
	sphereMatrixJupiter = sphereMatrixJupiter * glm::translate(glm::vec3(35, 0, 0)) * glm::rotate(glm::radians(75.0f * time), glm::vec3(0.0, 35.0, 0.0)) * glm::scale(glm::vec3(2.15f));

	drawObjectTexture(&sphereModel, sphereMatrixJupiter, textureJupiter);

	//Neptun
	glm::mat4 sphereMatrixNeptun = glm::translate(glm::vec3(0, 0, 0)) * glm::rotate(glm::radians(25.0f * time), glm::vec3(1, 12, -1));
	sphereMatrixNeptun = sphereMatrixNeptun * glm::translate(glm::vec3(55, 0, 0)) * glm::rotate(glm::radians(55.0f * time), glm::vec3(1, 12, -1)) * glm::scale(glm::vec3(1.3f));

	drawObjectTexture(&sphereModel, sphereMatrixNeptun, textureNeptun);


	glutSwapBuffers();
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	programColor = shaderLoader.CreateProgram("shaders/shader_color.vert", "shaders/shader_color.frag");
	programTexture = shaderLoader.CreateProgram("shaders/shader_tex.vert", "shaders/shader_tex.frag");
	programBackground = shaderLoader.CreateProgram("shaders/shader_tex_background.vert", "shaders/shader_tex_background.frag");
	sphereModel = obj::loadModelFromFile("models/sphere.obj");
	shipModel = obj::loadModelFromFile("models/spaceship.obj");
	chicken = obj::loadModelFromFile("models/Chicken.obj");
	textureSun = Core::LoadTexture("textures/sun.png"); //sun
	textureEarth = Core::LoadTexture("textures/earth.png"); //earth
	textureStars = Core::LoadTexture("textures/stars.png"); //stars
	textureJupiter = Core::LoadTexture("textures/jupiter.png"); //jupiter
	textureNeptun = Core::LoadTexture("textures/neptune.png"); //neptune
	//textureShip = Core::LoadTexture("textures/ship.png"); //ship
	textureChicken = Core::LoadTexture("textures/chicken.png");

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
	glutInitWindowSize(600, 600);
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
