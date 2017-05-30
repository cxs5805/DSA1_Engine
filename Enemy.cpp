#include "Enemy.h"
#include "Engine.h"
#include "Player.h"
#include "Bullet.h"

Enemy::Enemy()
{
	transform.rigidBody.vel = {-1.0f, 0, 0};
	collider = Collider::Sphere;
}

Enemy::Enemy(glm::vec3 loc, glm::vec3 vel, glm::vec3 force, float mass, glm::vec3 rot, glm::vec3 scal, bool render)
	: Object(loc, vel, force, mass, rot, scal, render)
{
	collider = Collider::Sphere;
	start = glm::vec3(30.0f, 5.0f, 0.0f);
	mag = 5.0f;
	freq = 9.0f;
	transform.location.y = start.y;
	health = maxHealth;
	respawnTimer = 0;
	removed = false;
}

Enemy::~Enemy()
{
}

void Enemy::move()
{
	// DEBUG
	//std::cout << "Enemy\n";

	
	if (health > 0)
	{
		// move to the left on every frame
		transform.location.x -= 0.02f;

		// velocity should already be set, and it won't change :^)
		wrap();
		oscillate();
		rotate();
	}
	else
	{
		// DEBUG
		//std::cout << "dead!\n";

		// stop applying force upon landing
		if (transform.location.y <= 0 && !removed)
		{
			// set flag to false
			removed = true;

			// fix position, velocity, and force to 0
			transform.location.y = 0.0f;
			transform.rigidBody.vel.y = 0.0f;
			transform.rigidBody.force.y = 0.0f;

			// remove
			remove();
		}

		// respawn handler
		if (removed)
		{
			// increment respawn timer
			//respawnTimer++;
			respawnTimer += 1000 * Engine::dt;

			// after a certain amount of time
			if (respawnTimer >= 2000)
			{
				// reset position and health, then "un-remove"
				this->transform.location = start;
				health = maxHealth;
				removed = false;

				// also reset timer
				respawnTimer = 0;
			}
		}
	}

	// DEBUG
	/*std::cout <<
		"\nPos = (" << transform.location.x <<
		", " << transform.location.y <<
		", " << transform.location.z << "\n-\n";
	*/
	//std::cout << "YPos = " << transform.location.y << "\n";
}

void Enemy::wrap()
{
	// if enemy exits the camera's left edge, wrap back to right edge
	if (transform.location.x <= 0.0f)
	{
		transform.location.x = start.x; // figure out what this start value is!
		
		// DEBUG
		//std::cout << "Wrapped!\n";
	}
}

void Enemy::oscillate()
{
	// oscillation is define by a sine wave
	// the enemy has a preset y-position...
	// so the sine should multiply by an arbitrary factor...
	// and then add to y-position
	float dy = glm::sin(Engine::sim_t * freq) * mag;
	transform.location.y = start.y + dy;

	//std::cout << "YDelta = " << dy << "\n-\n";
}

void Enemy::rotate()
{
	// rotate about y-axis
	transform.rotation.y += 0.02f;
}

/*
void Enemy::hit()
{
	// decrement health
	health--;

	// check death condition
	if (health <= 0)
	{
		// DEBUG
		//std::cout << "enemy died!\n";

		die();
	}
}
*/

void Enemy::die()
{
	// apply an upwards delta v, pop it up in the air
	transform.rigidBody.vel.y = 5.0f;
	
	// apply a downwards force 
	transform.rigidBody.force.y = -20.0f;
}

void Enemy::remove()
{
	this->transform.location = glm::vec3(-200, -200, -200);
}
