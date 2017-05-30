// could do #include with "", but problem w/file directories
// <> looks in a list of possible locations,
// so tell project -> properties
// VC++ directories
// include directories
// $(ProjectDir)include
// now project has one extra place to look

// what to add
// include folder
// headers from all 4 external libraries
// all dlls and bins

#include <GL\glew.h>

#include <GLFW\glfw3.h>

#include <FreeImage.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>

#include <iostream>
#include <vector>

#include "ShaderProgram.h"
#include "Engine.h"
#include "Camera.h"

int main()
{
	// PURE VIRTUAL TEST, THIS SHOULD CAUSE COMPILER ERRORS
	//Object obj;

	// Slideset B - Opening a Window
	// try to init GLFW
	if (glfwInit() != GL_TRUE)
	{
		printf("Failed to init GLFW\n");
	}

	// initialize the window library
	if (glfwInit() == GL_FALSE)
	{
		return -1;
	}

	// create a windowed mode window
	GLFWwindow* GLFWwindowPtr =	glfwCreateWindow(800, 600, "Chris Schiff DSA1 Engine", NULL, NULL);

	// make it the currently active window or quit
	if (GLFWwindowPtr != nullptr)
	{
		glfwMakeContextCurrent(GLFWwindowPtr);
	}
	else
	{
		glfwTerminate();
		return -1;
	}

	// initialize GLEW or quit
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return -1;
	}

	// 3/30/17 HW week 9
	// create engine and model
	Engine myEngine = Engine();

	// hide cursor
	glfwSetInputMode(GLFWwindowPtr, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// enable depth buffering
	glEnable(GL_DEPTH_TEST);

	// set window color to cornflower blue
	// only needs to be called once
	// you'll see it, provided no skybox
	//glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// FINALS WEEK
	// pause button flag
	bool pause = false;
	int prevKeyState = 0;
	int currKeyState = 0;

	// GAME LOOP
	// loop until user closes window
	while (!glfwWindowShouldClose(GLFWwindowPtr))
	{
		// clear canvas
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears color buffer
		// takes current canvas and wipes it with set color
		// you'll almost always clear canvas and depth buffer at the same time

		// WEEK 9
		// update camera matrix
		myEngine.update(GLFWwindowPtr, pause);
		// WEEK 10: this also calls myModel.render();

		// FINALS WEEK
		// update key state
		prevKeyState = currKeyState;
		currKeyState = glfwGetKey(GLFWwindowPtr, GLFW_KEY_ENTER);
		
		// press Enter to pause and start pause timer
		if (currKeyState && prevKeyState == 0)
		{
			pause = !pause;
			
			// paused - set time of pause
			if (pause)
				myEngine.time_of_pause = myEngine.t;
			// unpaused - increment sim clock by dt to resync it with comp clock
			else
				myEngine.sim_t += myEngine.dt;
		}
		
		// swap the front (what the screen displays)
		// and back (what OpenGl draws to) buffers
		// can't read and write from same place at same time
		glfwSwapBuffers(GLFWwindowPtr);

		// sample stuff for OpenGL game loop
		// update physical simulation
		// draw buffered models
		// process input/window events
		// process queued window, mouse and keyboard callback events
		glfwPollEvents();
	}

	// deallocate shader program from graphics card
	/*myShaderProgram.unload();
	myShaderProgram.~ShaderProgram();*/
	//myEngine.myModel.tex.unload();
	
	glfwTerminate();
	return 0;
}
