#include <iostream>
#include "Object.h"
#pragma once
class Enemy :
	public Object
{
private:
	// constants
	glm::vec3 start; // enemy's starting x-value
	float mag; // magnitude of oscillation
	float freq; // frequency of oscillation

	// move method and its helper methods
	void move() override;
	void wrap();
	void oscillate();
	void rotate();

	// health helper members and methods
	int maxHealth = 1;
	void die() override;
	int respawnTimer;
	bool removed;
	void remove();

public:
	Enemy();
	Enemy(glm::vec3 loc, glm::vec3 vel, glm::vec3 force, float mass, glm::vec3 rot, glm::vec3 scal, bool render);
	~Enemy();

	// health member and methods
	//int health;
	//void hit() override;
};
