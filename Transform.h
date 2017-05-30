#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <FreeImage.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>

#include "Model.h"
#pragma once
class Transform
{
private:
	// helper methods
	void upload();
	void physicsUpdate();
public:
	Transform();
	Transform(glm::vec3 loc, glm::vec3 vel, glm::vec3 force, float mass, glm::vec3 rot, glm::vec3 scal);
	~Transform();

	// member data
	glm::vec3 location;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 mat;
	Rigidbody rigidBody;

	// methods
	void update(GLFWwindow* window);

};
