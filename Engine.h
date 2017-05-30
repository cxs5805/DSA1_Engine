#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <FreeImage.h>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\euler_angles.hpp>

//#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Object.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "NewCamera.h"
#include "Transform.h"
#include "ShaderProgram.h"
#include <string>
#include <vector>

#pragma once
class Engine
{
private:
	//static std::string myModelFilename;
	static glm::vec3 lightLoc;

	// helper methods
	static void start();
	static void stop();
	static bool bufferModels();
	static void getDeltaTime();
	static void updateSimTime();
	
public:
	Engine();
	~Engine();

	// methods
	static void update(GLFWwindow* window, bool pause);
	// collision method
	//static bool colliding(Object object1, Object object2);

	// members
	static std::vector<Obj> objs;
	static std::vector<Object*> objects;
	//static Camera myCamera;
	static ShaderProgram myShaderProgram;
	//static Model myModel;
	
	// AFTER FINALS
	// computer clock members
	static float t0;
	static float t;
	static float dt;
	
	// simulation clock members
	static float sim_t0;
	static float sim_t;

	// pause time helper member
	static float pause_t;

	// helper member to get comp time at which pause occurs
	static float time_of_pause;

	//static Texture myTexture;
};

