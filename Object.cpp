#include "Object.h"
#include "Engine.h"

Object::Object()
{
	// have a default transform
	transform = Transform();

	// assume the object should be rendered
	shouldRender = true;

	// assume no collider (can be specified in child classes)
	// player = sphere
	// enemy = sphere
	// bullet = aabb
	// platform = aabb
	collider = Collider::Colliderless;
}

Object::Object(glm::vec3 loc, glm::vec3 vel, glm::vec3 force, float mass, glm::vec3 rot, glm::vec3 scal, bool render)
{
	transform = Transform(loc, vel, force, mass, rot, scal);
	Object::shouldRender = render;
}


Object::~Object()
{
}

void Object::update()
{
	// first, move the object (will vary based on whether it's a Player, Enemy, Bullet, or NewCamera)
	move();

	// update the transform (first does physics update, then matrix update, then upload)
	transform.update(glfwGetCurrentContext());

	// render
	//if (shouldRender) render();
}

void Object::move()
{
}

void Object::render()
{
	// now render and bind texture
	model.render();
	glBindTexture(GL_TEXTURE_2D, model.tex.texID);
}

bool Object::collidesWith(const Object &object)
{
	// DEBUG
	//std::cout << "In collidesWith()\n";

	// general rule of thumb for this method and its helper methods
	// false = NO collision
	// true = YES collision

	// collider check
	if (collider == Collider::Colliderless || object.collider == Collider::Colliderless)
		return false;

	// get distance between two objects, MANDATORY FOR ALL COLLISIONS
	glm::vec3 distVec = transform.location - object.transform.location;

	// DEBUG
	//std::cout << "Dist ="
	//	<< " (" << distVec.x
	//	<< ", " << distVec.y
	//	<< ", " << distVec.z << ")\n";

	// sphere-sphere
	if (collider == Collider::Sphere && object.collider == Collider::Sphere)
		return sphereSphere(object, distVec);

	// AABB-AABB
	if (collider == Collider::AABB && object.collider == Collider::AABB)
		return twoAABB(object, distVec);

	// sphere-AABB
	if (collider == Collider::Sphere && object.collider == Collider::AABB)
		return sphereAABB(object, distVec);

	// AABB-sphere
	if (collider == Collider::AABB && object.collider == Collider::Sphere)
		return object.sphereAABB(*this, -distVec);

	return true;
}

bool Object::sphereSphere(const Object & object, glm::vec3 distVec)
{
	// sum of two radii
	// just using x-scale for scaling b/c it's identical in each axis
	//float sumRad = transform.scale.x + object.transform.scale.x;
	float sumRad =
		(transform.scale.x * model.maxR) +
		(object.transform.scale.x * object.model.maxR);
	
	// now square the sum, makes calculation faster b/c no square root
	sumRad *= sumRad;

	// distance between two objects dotted with itself
	//glm::vec3 distVec = object.transform.location - transform.location;
	float distDot = glm::dot(distVec, distVec);
	
	// OLD VERSION OF DISTANCE
	//float dist = glm::distance(object.transform.location, transform.location);
	
	// DEBUG
	//std::cout << "Distance dotted with itself = " << distDot <<
	//	"\nSum of radii squared = " << sumRad << "\n-\n";

	// if distance dotted with distance is greater than sum radii squared...
	if (distDot > sumRad)
		// ...they can't be colliding
		return false;
	// dist (dot) dist > sumRad * sumRad
	return true;
}

bool Object::twoAABB(const Object & object, glm::vec3 distVec)
{
	// get max box radius for each object
	glm::vec3 e_a = transform.scale * model.maxXYZ;
	glm::vec3 e_b = object.transform.scale * object.model.maxXYZ;

	// get distance between two objects
	//glm::vec3 distVec = transform.location - object.transform.location;
	
	// DEBUG
	//std::cout << "Abs val of x-dist = " << glm::abs(distVec.x) <<
	//	"\nSum of abs vals of e_a and e_b = " << glm::abs(e_a.x) + glm::abs(e_b.x) << "\n-\n";

	// check all 3 axes for collision
	if
		(
		glm::abs(distVec.x) > (glm::abs(e_a.x) + glm::abs(e_b.x)) ||
		glm::abs(distVec.y) > (glm::abs(e_a.y) + glm::abs(e_b.y)) ||
		glm::abs(distVec.z) > (glm::abs(e_a.z) + glm::abs(e_b.z))
		)
		return false;

	return true;
}

bool Object::sphereAABB(const Object & object, glm::vec3 distVec) const
{
	// get radius of sphere, then square
	float r_a = transform.scale.x * model.maxR;
	r_a *= r_a;

	// get max box radius for box
	glm::vec3 e_b = object.transform.scale * object.model.maxXYZ;

	// get closest point on box to sphere
	glm::vec3 p = object.transform.location + glm::clamp(distVec, -e_b, e_b);

	// get distance from sphere center to p, then dot with itself
	glm::vec3 d_p = transform.location - p;
	float d_pDot = glm::dot(d_p, d_p);

	// DEBUG
	//std::cout << "Dist = " << glm::length(d_p) << "\nDist dotted w/itself = " << d_pDot <<
	//	"\nSphere radius squared = " << r_a << "\n-\n";

	// if distance to p dotted with itself is greater than radius squared, 
	// then no collision
	if (d_pDot > r_a) return false;

	return true;
}
/*
bool Object::AABBSphere(const Object & object, glm::vec3 distVec)
{
	// get radius of sphere, then square
	float r_b = object.transform.scale.x * object.model.maxR;
	r_b *= r_b;

	// get max box radius for box
	glm::vec3 e_a = transform.scale * model.maxXYZ;

	// get closest point on box to sphere
	glm::vec3 p = transform.location + glm::clamp(distVec, -e_a, e_a);

	// get distance from sphere center to p, then dot with itself
	glm::vec3 d_p = object.transform.location - p;
	float d_pDot = glm::dot(d_p, d_p);

	// DEBUG
	//std::cout << "Dist = " << glm::length(d_p) << "\nDist dotted w/itself = " << d_pDot <<
	//	"\nSphere radius squared = " << r_b << "\n-\n";

	// if distance to p dotted with itself is greater than radius squared, 
	// then no collision
	if (d_pDot > r_b) return false;

	return true;
}
*/

void Object::hit()
{
	// decrement health
	health--;

	// check death condition
	if (health <= 0) die();
}

void Object::die()
{
}
