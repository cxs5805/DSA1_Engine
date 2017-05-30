#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <FreeImage.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>

#include "Model.h"
#include "Transform.h"
#include <string>

#pragma once

// enum for determining what type of collider an object has
enum Collider
{
	Colliderless,
	Sphere,
	AABB,
	OBB
};

class Object
{
private:
	// methods
	virtual void move() = 0; // PURE VIRTUAL TEST, HOPEFULLY THIS MAKES THE CLASS ABSTRACT
	//void render();

	// helper methods for the collidesWith() method
	bool sphereSphere(const Object &object, glm::vec3 distVec);
	bool twoAABB(const Object &object, glm::vec3 distVec);
	bool sphereAABB(const Object &object, glm::vec3 distVec) const;
	//bool AABBSphere(const Object &object, glm::vec3 distVec);

	// death helper method
	virtual void die();

public:
	Object();
	Object(glm::vec3 loc, glm::vec3 vel, glm::vec3 force, float mass, glm::vec3 rot, glm::vec3 scal, bool render);
	virtual ~Object();

	// methods
	void update();

	// Transform (includes Rigidbody and physics update)
	Transform transform;

	// model member data
	Model model;
	std::string modelFilename;

	// collider member data and collision method
	Collider collider;
	bool collidesWith(const Object &object);

	// flag to determine whether to render the object
	// (false for Camera;
	// true for Player, Enemy, and Bullet)
	bool shouldRender;
	void render();

	// health member and methods
	int health;
	void hit();
};

