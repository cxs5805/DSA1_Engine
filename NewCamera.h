#include "Object.h"
#include <iostream>

#pragma once
class NewCamera :
	public Object
{
private:
	// members
	float speed = 1.5f;
	float sensitivity = 0.005f;
	glm::mat4 worldView;

	// flag to determine whether to use old camera movement algorithms
	// normally is false, but make true if you need to debug
	bool debug = false;

	// perspective matrix members
	float zoom = 1.0f;
	int width = 800;
	int height = 600;
	float fovy = 3.14159f * 0.4f / zoom; // field of view y
	float aspect = (float)width / (float)height;
	float zNear = 0.01f;
	float zFar = 1000.0f;
	glm::mat4 perspectiveMat =
		glm::perspective(fovy, aspect, zNear, zFar);

	// move method and its helper methods
	void move() override;
	void updateMatrix();
	void uploadMatrix();
	void toggleDebug(GLFWwindow* window);

	// old
	void debugAlgs(GLFWwindow* window);
	void oldMove(GLFWwindow* window);
	void turn(GLFWwindow* window);

	// new
	void presetAngles(GLFWwindow* window);

public:
	NewCamera();
	NewCamera(glm::vec3 loc);
	~NewCamera();

	glm::mat3 rotMat;
};

