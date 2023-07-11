#include <GL/glew.h>
#include "body.cpp"
#include "shader_s.h"
#include <GLFW/glfw3.h>
#include <array>

#include <iostream>

#define SCR_WIDTH 1600
#define SCR_HEIGHT 800

#define maxDiam 70

void updateSim(Body *b1, Body *b2, GLfloat* pointSize);
GLfloat* pointSizeCalc(double m1, double m2);

int main(void)
{
	GLFWwindow* window;

	//initialize lib
	if (!glfwInit())
	{
		return -1;
	}

	//create windowed
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GRRRRavity", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	//make windows current context
	glfwMakeContextCurrent(window);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); //especificamos donde se drawea opengl
	glMatrixMode( GL_PROJECTION); //la matrix de proyeccion define como funciona la camara
	glLoadIdentity();
	glOrtho(0, SCR_WIDTH, 0, SCR_HEIGHT, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//celeste b1
	Body* b1 = new Body(0, 6.0 * std::pow(10, 24), { 0, 0 }, { SCR_WIDTH / 2, SCR_HEIGHT / 2});
	//blanco b2
	Body* b2 = new Body(1, 7.4 * std::pow(10, 22), { 1000000, -100 }, { SCR_WIDTH / 2, SCR_HEIGHT * 0.9 });

	//std::cout << "1: " << b1->mass << "\t2: " << b2->mass; std::getchar();

	GLfloat pointVertex[] = { b1->position[0], b1->position[1] };
	GLfloat pointVertex2[] = { b2->position[0], b2->position[1] };

	GLfloat pointSize[2];
	if (b1->mass > b2->mass)
	{
		pointSize[0] = maxDiam;
		pointSize[1] = maxDiam * (1 - std::log10(b2->mass)/std::log10(b1->mass)) / 0.2;
		std::cout << "1: " << pointSize[0] << "\t2: " << pointSize[1]; 
		std::getchar();
	}
	else
	{
		pointSize[0] = b1->mass * 70 / b2->mass;
		pointSize[1] = maxDiam;
	}



	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		//std::cout << "Before " << b1->position[0] << "\t" << b1->position[1] << std::endl;

		updateSim(b1, b2, pointSize);

		//std::cout << "Posicion x:\nb1 = " << b1->position[0] << "\nb2 = " << b2->position[0];
		//std::cout << "\nPosicion y:\nb1 = " << b1->position[1] << "\nb2 = " << b2->position[1];

		GLfloat pointVertex[] = { b1->position[0], b1->position[1] };
		GLfloat pointVertex2[] = { b2->position[0], b2->position[1] };

		//renderizamos opengl
		glEnable(GL_POINT_SMOOTH); //make the point circular
		glEnableClientState(GL_VERTEX_ARRAY);

		//b1 celeste
		glColor3f(0.3f, 0.5f, 1.0f);
		glPointSize(pointSize[0]);
		glVertexPointer(2, GL_FLOAT, 0, pointVertex);
		glDrawArrays(GL_POINTS, 0, 1); //draw vertices
		
		//b2 blanco
		glPointSize(pointSize[1]);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertexPointer(2, GL_FLOAT, 0, pointVertex2);
		glDrawArrays(GL_POINTS, 0, 1);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisable(GL_POINT_SMOOTH);

		//swap buffers
		glfwSwapBuffers(window);

		//poll events
		glfwPollEvents();
	}

	glfwTerminate();
	
	return 0;

	
}

void updateSim(Body *b1, Body *b2, GLfloat* pointSize)
{
	glm::dvec2 r = Body::distance(b1->position[0], b2->position[0], b1->position[1], b2->position[1]);

	/*
	if ((std::abs(r[0]) > 0 && std::abs(r[0]) < 1) && (std::abs(r[1]) > 0 && std::abs(r[1]) < 1))
	{
		std::cout << "\n===Collission detected===\n";
		std::cin.ignore();
	}
	*/

	double mod = std::sqrt(pow(r[0], 2) + pow(r[1], 2));
	if ((pointSize[0] + pointSize[1])/2 >= mod)
	{
		std::cout << "\n===Collission detected===\n";
		std::cin.ignore();
	}

	b1->gForce(b2->mass, r);
	b2->force = -b1->force;

	b1->update(0.000001);
	b2->update(0.000001);

	std::cout << "B1 " << b1->position[0] << "\t" << b1->position[1] << std::endl;
	std::cout << "B2 " << b2->position[0] << "\t" << b2->position[1] << std::endl;
	std::cout << "R " << r[0] << "\t" << r[1] << "\n\n";
	//std::cout << "Force " << b1->force[0] << "\t" << b1->force[1] << std::endl;
	//std::cout << "Distance " << r[0] << "\t" << r[1] << std::endl;
	//std::cin.ignore();
}

//calcular el tama;o de punto conociendo las masas
GLfloat* pointSizeCalc(double m1, double m2)
{
	GLfloat arr[2];
	if (m1 > m2)
	{
		arr[0] = 70;
		arr[1] = m2 * 70/ m1;
	}
	else
	{
		arr[0] = m1 * 70 / m2;
		arr[1] = 70;
	}

	return arr;
}

GLfloat normalizedPos2SCR(double pos, int scr)
{
	return 0;
}