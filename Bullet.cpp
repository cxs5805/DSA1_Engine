#include "Bullet.h"
#include "Engine.h"

Bullet::Bullet()
{
}

Bullet::Bullet(glm::vec3 loc, float speed, bool shouldMove)
	: Object(loc, glm::vec3(speed, 0, 0), glm::vec3(), 1.0f, glm::vec3(), glm::vec3(1), true)
{
	this->shouldMove = shouldMove;
	xSpeed = speed;
	collider = Collider::AABB;
}

Bullet::~Bullet()
{
}

void Bullet::move()
{
	// check if should move
	if (!shouldMove)
		transform.rigidBody.vel.x = 0;
	else
		transform.rigidBody.vel.x = xSpeed;

	// no forces applied to the bullet, so no extra code for movement
	// collisions will happen in the future

	// DEBUG
	//std::cout <<
	//	"Bullet" <<
	//	"\nPosition = (" << transform.location.x <<
	//	", " << transform.location.y <<
	//	", " << transform.location.z <<
	//	"\n-\n";
}
