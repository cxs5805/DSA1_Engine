#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <FreeImage.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>

// engine include is in the .cpp file for this class
#include <iostream>

#pragma once
class Camera
{
private:
	// members
	float speed = 1.5f;
	float sensitivity = 0.005f;
	glm::mat3 rotMat;
	glm::vec3 velocity;
	glm::vec3 location;
	glm::vec3 rotation;
	glm::mat4 worldView;

	// perspective matrix members
	float zoom = 1.0f;
	int width = 800;
	int height = 600;
	float fovy = 3.14159f * 0.4f / zoom; // field of view y
	float aspect = (float)width / (float) height;
	float zNear = 0.01f;
	float zFar = 1000.0f;
	glm::mat4 perspectiveMat = 
		glm::perspective(fovy, aspect, zNear, zFar);

public:
	Camera();
	~Camera();

	// methods
	void start();
	void update(GLFWwindow* window);
	void upload();
	void move(GLFWwindow* window);
	void turn(GLFWwindow* window);
};

