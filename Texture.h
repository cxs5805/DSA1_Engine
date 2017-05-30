#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <FreeImage.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>

#include <string>
#include <iostream>

#pragma once
class Texture
{
public:
	Texture();
	~Texture();

	// methods
	void bind();
	void unload();
	void load();

	// public members
	GLuint texID; // gets a value from glGenTextures
	//const char* texFile = "textures/TestTexture.png";
	std::string texFile = "textures/TestTexture.png";
};
