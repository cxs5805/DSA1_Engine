#include <iostream>
#include "Object.h"
#include "Bullet.h"
#include "Enemy.h"

#pragma once
class Player :
	public Object
{
private:
	// flag to determine if in midair
	bool up;

	// friction constant
	const float friction = 0.05f;
	
	// velocity constant and helper method to cap velocity
	const float maxVel = 12.0f;
	void capVel();

	// move method and its helper methods
	void move() override;
	void airCheck();
	void validateInput();
	void keepInBounds();
	void shoot();
	void applyFriction(GLFWwindow* window);

	// bullet helper members and methods
	int bulletTimer = 0;
	int bulletLifespan = 1500;
	bool canFire = true;
	void removeBullet(Bullet* b);
	void manageBullets();

	// health members and methods
	int invulDuration = 1000;
	int invulTimer;
	void die() override;
	void beInvul();

	// score members and methods
	int score = 0;
	int scoreToWin = 5;
	void scorePoint();

	// key state members for A and D
	int prevKeyStates[2];
	int currKeyStates[2];

public:
	Player();
	Player(glm::vec3 loc, glm::vec3 vel, glm::vec3 force, float mass, glm::vec3 rot, glm::vec3 scal, bool render);
	~Player();


	// health member and methods
	bool invul;
};
