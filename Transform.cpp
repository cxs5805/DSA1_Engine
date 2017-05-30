#include "Transform.h"
#include "Engine.h"


Transform::Transform()
{
	// velocity and force defeault to zero, and object can't have nonzero mass
	rigidBody.mass = 1.0f;
	mat = glm::mat4();
}

Transform::Transform(glm::vec3 loc, glm::vec3 vel, glm::vec3 force, float mass, glm::vec3 rot, glm::vec3 scal)
{
	location = loc;
	rigidBody.vel = vel;
	rigidBody.force = force;
	rigidBody.mass = mass;
	rotation = rot;
	scale = scal;
	mat = glm::mat4();
}


Transform::~Transform()
{
}

void Transform::update(GLFWwindow * window)
{
	// update transform's physics based on current net force
	physicsUpdate();

	// update model's transforms in scene BEFORE RENDERING
	mat = (
		glm::translate(location)
		* glm::yawPitchRoll(rotation.y, rotation.x, rotation.z)
		//* glm::rotate((float)0, rotation)
		* glm::scale(scale));

	// now we can upload the new transform matrix to the shader program
	upload();

	// DEBUG
	/*std::cout
	<< "object number " << i + 1
	<< "\nlocation\t= (" << location.x << ", " << location.y << ", " << location.z << ")"
	<< "\nrotation\t= (" << rotation.x << ", " << rotation.y << ", " << rotation.z << ")"
	<< "\nscale\t\t= (" << scale.x << ", " << scale.y << ", " << scale.z << ")\n-\n";
	*/
}

void Transform::physicsUpdate()
{
	// update velocity and position based on force and delta time
	rigidBody.vel += rigidBody.force * Engine::dt / rigidBody.mass;
	location += rigidBody.vel * Engine::dt;
}

void Transform::upload()
{
	// regular objects
	// upload new transform to vertex shader
	glUniformMatrix4fv(2, 1, GL_FALSE, &mat[0][0]);

	// CAMERA
	// upload transform location to shader
	// (fine for this project because only 1 shader program)
	//glUniform3fv(7, 1, &location[0]);
}
