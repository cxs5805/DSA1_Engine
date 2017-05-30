#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <FreeImage.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>

#include <string>
#include <fstream>
#include <iostream>
using namespace std;

#pragma once
class ShaderProgram
{
private:
	// unsigned integers
	GLuint program, vprogram, fprogram;
	string filenamev = "shaders/vshader.glsl";
	string filenamef = "shaders/fshader.glsl";
public:
	ShaderProgram();
	~ShaderProgram();

	// methods
	bool load();
	bool compile(GLenum shaderType);
	void use();
	void unload();
};
