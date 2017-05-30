#include <GL\glew.h>

#include <GLFW\glfw3.h>

#include <FreeImage.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "Texture.h"

#pragma once
class Model
{
private:
	// vertArr and vertCount, from main
	unsigned int vertCount;
	GLuint vertArr;
public:
	Model();
	~Model();

	// methods, from main
	bool buffer(std::string objFile);
	void render();

	// public members
	Texture tex;
	glm::vec3 maxXYZ; // vec3 to hold max x, y and z values for the model
	float maxR; // float to hold the largest value of maxXYZ, be it x y or z
};

struct Vertex
{
	glm::vec3 location;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct VertInd
{
	unsigned int locInd, uvInd, normInd;
};

struct Rigidbody
{
	glm::vec3 vel;
	glm::vec3 force;
	float mass;
};

struct Transf
{
	glm::vec3 location;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 mat;
	Rigidbody rigidBody;
};

struct Obj
{
	Model model;
	Transf transform;
	std::string texFilename;
};
