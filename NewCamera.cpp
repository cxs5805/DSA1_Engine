#include "NewCamera.h"
#include "Engine.h"

// remember, DO NOT render the camera
NewCamera::NewCamera()
	: Object(glm::vec3(0, 0, 2), glm::vec3(), glm::vec3(), 1.0f, glm::vec3(), glm::vec3(), false)
{
	// give worldview matrix the identity matrix
	worldView = glm::mat4();
	collider = Collider::Colliderless;
}

NewCamera::NewCamera(glm::vec3 loc)
	: Object(loc, glm::vec3(), glm::vec3(), 1.0f, glm::vec3(),glm::vec3(), false)
{
	// give worldview matrix the identity matrix
	worldView = glm::mat4();
	collider = Collider::Colliderless;
}

NewCamera::~NewCamera()
{
}

void NewCamera::move()
{
	// DEBUG
	//std::cout << "NewCamera\n";

	// update worldview matrix and upload to shader program
	updateMatrix();
	uploadMatrix();

	// get current window to validate user input
	GLFWwindow* window = glfwGetCurrentContext();

	// get user input to toggle camera behavior
	toggleDebug(window);

	// move camera based on debug flag
	if (debug)
		debugAlgs(window);
	else
		presetAngles(window);

	// DEBUG
	//std::cout << "\n-\n";
}

void NewCamera::updateMatrix()
{
	// DEBUG
	//std::cout << "In updateMatrix()\n";

	// update worldview based on new camera position
	worldView = perspectiveMat * glm::lookAt(transform.location,
		transform.location + rotMat * glm::vec3(0, 0, -1),
		rotMat * glm::vec3(0, 1, 0));
	// remember, transformations apply from RIGHT TO LEFT!!!!!
}

void NewCamera::uploadMatrix()
{
	// DEBUG
	//std::cout << "In uploadMatrix()\n";

	// upload worldView matrix to vertex shader
	glUniformMatrix4fv(4, 1, GL_FALSE, &worldView[0][0]);
}

// press backspace to toggle between old and new camera behavior
void NewCamera::toggleDebug(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS)
	{
		debug = !debug;

		// DEBUG
		std::cout << "Debug changed! now = " << debug << "\n";
	}
		
}

void NewCamera::debugAlgs(GLFWwindow* window)
{
	// DEBUG
	//std::cout << "Using old camera movement\nIn debugAlgs()\n";

	oldMove(window);
	turn(window);
}

void NewCamera::oldMove(GLFWwindow* window)
{
	// DEBUG
	//std::cout << "In oldMove()\n";

	// create direction vector
	glm::vec3 dir = glm::vec3();

	// move via arrow keys, applying a delta v directly (NO FORCES)
	// left right
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		dir.x -= 1;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		dir.x += 1;

	// forward backward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		dir.z -= 1;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		dir.z += 1;

	// normalize if nonzero vector
	if (dir != glm::vec3()) dir = glm::normalize(dir);

	// update velocity
	transform.rigidBody.vel = rotMat * dir * speed;

	// update location
	//location += Engine::myTimer.dt * velocity;
	transform.location += Engine::dt * transform.rigidBody.vel;

	// DEBUG
	//std::cout <<
	//	"velocity = <" <<
	//	transform.rigidBody.vel.x << ", " <<
	//	transform.rigidBody.vel.y << ", " <<
	//	transform.rigidBody.vel.z << ">\n";
}

void NewCamera::turn(GLFWwindow* window)
{
	// DEBUG
	//std::cout << "In turn()\n";

	// get window size
	int w, h;
	glfwGetWindowSize(window, &w, &h);

	// get cursor position
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	// rotate about y
	//rotation.y += sensitivity * (w / 2) - x;
	transform.rotation.y -= sensitivity * (x - w * 0.5f);
	// about x
	//rotation.x += sensitivity * (h / 2) - y;
	transform.rotation.x -= sensitivity * (y - h * 0.5f);

	// clamp x rotation from -pi/2 to pi/2
	transform.rotation.x = glm::clamp(transform.rotation.x, -1.57f, 1.57f);

	// set cursor to center
	//glfwSetCursorPos(window, x, y);
	glfwSetCursorPos(window, w * 0.5f, h * 0.5f);

	// update rotation matrix
	rotMat = glm::mat3(glm::yawPitchRoll(transform.rotation.y, transform.rotation.x, transform.rotation.z));

	// DEBUG
	//std::cout <<
	//	"rotation = <" <<
	//	transform.rotation.x << ", " <<
	//	transform.rotation.y << ", " <<
	//	transform.rotation.z << ">\n";
}

void NewCamera::presetAngles(GLFWwindow* window)
{
	// DEBUG
	//std::cout << "Using new camera movement\nIn oldMove()\n";
}
