#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include<iostream>
#include<thread>
#include"Maze.h"
#include <GLFW/glfw3.h>
#include <gl/GL.h>

GLFWwindow* window;



/* program entry */
int main(int argc, char *argv[])
{


	Maze maze;
	
	std::thread t(&Maze::CreateMaze, &maze);
	t.detach();

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(700, 700, "LearnOpenGL", NULL, NULL);
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}


	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// set up view
	glViewport(0, 0, 700, 700);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// see https://www.opengl.org/sdk/docs/man2/xhtml/glOrtho.xml
	glOrtho(0.0, 700.0, 0.0, 700.0, 0.0, 1.0); // this creates a canvas you can do 2D drawing on


											   // Main loop
	while (!glfwWindowShouldClose(window))
	{

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPointSize(5);
		glLineWidth(2.5);
		glColor3f(1.0, 0.0, 0.0);
	
		maze.DrawMaze();
		

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		




	}
	// Terminate GLFW
	glfwTerminate();

	// Exit program
	exit(EXIT_SUCCESS);
}