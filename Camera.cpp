#include "Camera.h"
#include "Engine.h" // this line can't be in Camera.h

Camera::Camera()
{
	start();
}


Camera::~Camera()
{
}

void Camera::start()
{
	// initialize world-view matrix to identity matrix
	worldView = glm::mat4();
	location = { 0, 0, 2 };
	rotation = glm::vec3();
}

void Camera::update(GLFWwindow* window)
{
	/*// update delta t
	t0 = t;
	t = (float)glfwGetTime();
	dt = t - t0;*/
	//glUniform1fv(5, 1, &t);
	//std::cout << "dt = " << dt << "\n";
	
	// update worldview based on new camera position
	worldView = perspectiveMat * glm::lookAt(location,
		location + rotMat * glm::vec3(0, 0, -1),
		rotMat * glm::vec3(0, 1, 0));
	// remember, transformations apply from RIGHT TO LEFT!!!!!

	// upload worldView matrix to vertex shader
	upload();

	// update position and rotation
	move(window);
	turn(window);

	//std::cout << "location = <" << location.x << ", "
	//	<< location.y << ", "
	//	<< location.z << ">\n";
}

void Camera::upload()
{
	// upload worldView matrix to vertex shader
	glUniformMatrix4fv(4, 1, GL_FALSE, &worldView[0][0]);

	// Week 11 - upload camera location
	glUniform3fv(7, 1, &location[0]);
}

void Camera::move(GLFWwindow* window)
{
	// create direction vector
	glm::vec3 dir = glm::vec3();

	// left right
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		dir.x -= 1;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		dir.x += 1;

	// forward backward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		dir.z -= 1;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		dir.z += 1;

	// normalize if nonzero vector
	if (dir != glm::vec3()) dir = glm::normalize(dir);

	// update velocity
	velocity = rotMat * dir * speed;
	
	// update location
	//location += Engine::myTimer.dt * velocity;
	location += Engine::dt * velocity;

	//std::cout << "velocity = <" << velocity.x << ", "
	//	<< velocity.y << ", "
	//	<< velocity.z << ">\n";
}

void Camera::turn(GLFWwindow * window)
{
	// get window size
	int w, h;
	glfwGetWindowSize(window, &w, &h);

	// get cursor position
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	// rotate about y
	//rotation.y += sensitivity * (w / 2) - x;
	rotation.y -= sensitivity * ( x - w * 0.5f);
	// about x
	//rotation.x += sensitivity * (h / 2) - y;
	rotation.x -= sensitivity * (y - h * 0.5f);

	// clamp x rotation from -pi/2 to pi/2
	rotation.x = glm::clamp(rotation.x, -1.57f, 1.57f);

	// set cursor to center
	//glfwSetCursorPos(window, x, y);
	glfwSetCursorPos(window, w * 0.5f, h * 0.5f);

	// update rotation matrix
	rotMat = glm::mat3(glm::yawPitchRoll(rotation.y, rotation.x, rotation.z));

	//std::cout << "rotation = <" << rotation.x << ", "
	//	<< rotation.y << ", "
	//	<< rotation.z << ">\n";
}
