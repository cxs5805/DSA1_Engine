#include <iostream>
#include "Object.h"
#include "Enemy.h"
#pragma once
class Bullet :
	public Object
{
private:
	void move() override;

public:
	Bullet();
	Bullet(glm::vec3 loc, float speed, bool shouldMove);
	~Bullet();

	// flag to determine if the bullet should move
	bool shouldMove;

	// member to keep track of bullet speed
	float xSpeed;
};

