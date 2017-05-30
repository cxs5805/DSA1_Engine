#include "Engine.h"

//Camera Engine::myCamera;
//Model Engine::myModel;
//std::string Engine::myModelFilename;
glm::vec3 Engine::lightLoc;
std::vector<Obj> Engine::objs;
std::vector<Object*> Engine::objects;
float Engine::t0, Engine::t, Engine::dt, Engine::sim_t0, Engine::sim_t, Engine::pause_t, Engine::time_of_pause;
//Texture Engine::myTexture;
ShaderProgram Engine::myShaderProgram;

Engine::Engine()
{
	start();
}


Engine::~Engine()
{
	stop();
}

void Engine::start()
{
	// OLD CAMERA
	//myCamera = Camera();

	// 3/23/17, HW week 8
	// create instance of ShaderProgram class
	myShaderProgram = ShaderProgram();

	// load, then check if not loaded
	bool result = myShaderProgram.load();
	std::cout << "Did it load? " << result << "\n";
	if (!result)
	{
		// keep console window open
		std::cout << "Well, you screwed up :/\n";
	}
	else
	{
		// now use as current rendering program
		myShaderProgram.use();
	}

	//myModel = Model();
	//myTexture = Texture();
	//myModelFilename = "models/sphere.obj";
	lightLoc = {0, 0, 5};

	// Week 11 - upload lightLoc to vertex shader ONCE because it doesn't move
	// UPLOAD UNIFORMS AFTER SHADER CREATED
	// THAT WAY, THIS WILL WORK!!!!!
	glUniform3fv(8, 1, &lightLoc[0]);

	// Week 13 - create objs
	/*
	// OLD STRUCT
	Obj o1;
	o1.texFilename = "models/quad.obj";
	o1.transform.location = {0, 0, -4};
	o1.transform.rotation = { (0.5f) * glm::pi<float>(), 0, 0};
	o1.transform.scale = {1, 1, 1};
	o1.transform.mat = glm::mat4();
	objs.push_back(o1);

	Obj o2;
	o2.texFilename = "models/quad.obj";
	o2.transform.location = {0, 0, 0};
	o2.transform.rotation = {0, 0, (0.5f) * glm::pi<float>()};
	o2.transform.scale = {2, 2, 2};
	o2.transform.mat = glm::mat4();
	objs.push_back(o2);

	Obj o3;
	o3.texFilename = "models/quad.obj";
	o3.transform.location = {0, 0, -2};
	o3.transform.rotation = {0, (0.5f) * glm::pi<float>(), 0};
	o3.transform.scale = {3, 3, 3};
	o3.transform.mat = glm::mat4();
	objs.push_back(o3);
	*/
	
	// Week 14
	// NEW CLASS
	/*
	Object* object1 = new Object
	(
		glm::vec3(10, 0, -4),
		glm::vec3(),
		glm::vec3(),
		1.0f,
		glm::vec3((0.5f) * glm::pi<float>(), 0, 0),
		glm::vec3(1, 1, 1),
		true
	);
	object1->transform.mat = glm::mat4();
	object1->modelFilename = "models/quad.obj";
	objects.push_back(object1);

	Object* object2 = new Object
	(
		glm::vec3(10, 0, 0),
		glm::vec3(),
		glm::vec3(),
		1.0f,
		glm::vec3(0, 0, (0.5f) * glm::pi<float>()),
		glm::vec3(2, 2, 2),
		true
	);
	object2->transform.mat = glm::mat4();
	object2->modelFilename = "models/quad.obj";
	objects.push_back(object2);

	Object* object3 = new Object
	(
		glm::vec3(10, 0, -2),
		glm::vec3(),
		glm::vec3(),
		1.0f,
		glm::vec3(0, (0.5f) * glm::pi<float>(), 0),
		glm::vec3(3, 3, 3),
		true
	);
	object3->transform.mat = glm::mat4();
	object3->modelFilename = "models/quad.obj";
	objects.push_back(object3);
	*/
	// NEW CAMERA
	
	// here's the tricky part: new camera
	NewCamera* newCamera = new NewCamera(glm::vec3(0.5f, 4.5f, 8.3f));
	newCamera->transform.rotation = glm::vec3(0, -44.6, 0);
	newCamera->rotMat = glm::mat3(glm::yawPitchRoll(newCamera->transform.rotation.y, newCamera->transform.rotation.x, newCamera->transform.rotation.z));
	objects.push_back(newCamera);

	// NEW CHILDREN
	
	Player* player = new Player
	(
		glm::vec3(10, 0, 0),
		glm::vec3(),
		glm::vec3(),
		1.0f,
		glm::vec3(),
		glm::vec3(1, 1, 1),
		true
	);
	player->transform.mat = glm::mat4();
	player->modelFilename = "models/sphere.obj";
	player->model.tex.texFile = "textures/TestTexture.png";
	player->model.tex.load();
	objects.push_back(player);

	// FINALS WEEK
	// create the bullet(s) once
	// then buffer once,
	// this way, when the player presses fire...
	// ...the bullet can just change its position to wherever the player is
	// and it will only move if it has (A) been spawned and (B) is visible on screen

	Bullet* b1 = new Bullet(glm::vec3(-100, -100, -100), 0.0f, false);
	b1->transform.mat = glm::mat4();
	b1->transform.scale = glm::vec3(0.2);
	b1->modelFilename = "models/sphere.obj";
	b1->model.tex.texFile = "textures/TestTexture.png";
	b1->model.tex.load();
	objects.push_back(b1);

	/*
	Bullet* b2 = new Bullet(glm::vec3(-100, -100, -100), 0.0f, false);
	b2->transform.mat = glm::mat4();
	b2->modelFilename = "models/teapot.obj";
	objects.push_back(b2);

	Bullet* b3 = new Bullet(glm::vec3(-100, -100, -100), 0.0f, false);
	b3->transform.mat = glm::mat4();
	b3->modelFilename = "models/teapot.obj";
	objects.push_back(b3);
	*/

	Enemy* enemy = new Enemy
	(
		glm::vec3(5, 0, 0),
		glm::vec3(),
		glm::vec3(),
		1.0f,
		glm::vec3(),
		glm::vec3(1, 1, 1),
		true
	);
	enemy->transform.mat = glm::mat4();
	enemy->modelFilename = "models/quad.obj";
	enemy->model.tex.texFile = "textures/TestTexture.png";
	enemy->model.tex.load();
	objects.push_back(enemy);

	if (bufferModels())
		std::cout << "Models successfully buffered!\n";
	else
		std::cout << "Well, you screwed up buffering :/\n";

	// DEBUG
	std::cout << "Player health = " << objects[1]->health << "\n";
}

void Engine::stop()
{
	//myCamera.~Camera();
	myShaderProgram.unload();
	myShaderProgram.~ShaderProgram();
	//myModel.~Model();
	//myTexture.~Texture();

	
	// NEW CLASS
	
	while (objects.size() > 0)
	{
		delete objects[objects.size() - 1];
		objects.pop_back();
	}
	
}

void Engine::update(GLFWwindow* window, bool pause)
{
	// update computer time and delta t
	getDeltaTime();

	// if unpaused, update simulation time
	if (!pause)
		updateSimTime();
	// else, it's paused, so increment pause timer
	else
		pause_t += dt;

	//std::cout << "c = " << t <<
	//	"\ns = " << sim_t <<
	//	"\n";// p = " << pause_t << "\n";
	
	/*
	// OLD STRUCT
	int numObjs = objs.size();
	for (int i = 0; i < numObjs; i++)
	{
		// update models' transforms in scene BEFORE RENDERING
		objs[i].transform.mat = (
			glm::translate(objs[i].transform.location)
			* glm::yawPitchRoll(objs[i].transform.rotation.y, objs[i].transform.rotation.x, objs[i].transform.rotation.z)
			//* glm::rotate((float)0, objs[i].transform.rotation)
			* glm::scale(objs[i].transform.scale));

		// upload new transform to vertex shader
		glUniformMatrix4fv(2, 1, GL_FALSE, &objs[i].transform.mat[0][0]);

		// DEBUG
		//std::cout
		//	<< "object number " << i + 1
		//	<< "\nlocation\t= (" << objs[i].transform.location.x << ", " << objs[i].transform.location.y << ", " << objs[i].transform.location.z << ")"
		//	<< "\nrotation\t= (" << objs[i].transform.rotation.x << ", " << objs[i].transform.rotation.y << ", " << objs[i].transform.rotation.z << ")"
		//	<< "\nscale\t\t= (" << objs[i].transform.scale.x << ", " << objs[i].transform.scale.y << ", " << objs[i].transform.scale.z << ")\n-\n";
		
		// now render and bind texture
		objs[i].model.render();
		glBindTexture(GL_TEXTURE_2D, objs[i].model.tex.texID);
	}
	*/
	// NEW CLASS
	
	int numObjects = objects.size();
	for (int i = 0; i < numObjects; i++)
	{
		// update objects transforms in scene BEFORE RENDERING
		if (!pause) objects[i]->update();
		if (objects[i]->shouldRender)
			objects[i]->render();
	}

	/*
	// AFTER FINALS
	// call each object's render seperately
	numObjects = objects.size();
	for (int i = 0; i < numObjects; i++)
		if (objects[i]->shouldRender) objects[i]->render();
	*/

	// WEEK 15: manually check collisions between one player and one enemy
	if(objects[1]->collidesWith(*objects[3]) && !((Player*)objects[1])->invul && objects[3]->health > 0)
	{
		objects[1]->hit();
		std::cout << "Player health = " << objects[1]->health << "\n";
		((Player*)objects[1])->invul = true;
	}

	// OLD CAMERA
	// update static members
	//myCamera.update(window);
	//myModel.render();

	// bind texture while drawing it
	//glBindTexture(GL_TEXTURE_2D, myModel.tex.texID);

	// hit escape to close
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// buffer all models in scene
bool Engine::bufferModels()
{
	// WEEK 10: only one model
	/*if (myModel.buffer(myModelFilename))
		return true;
	else
		return false;
	*/

	// OLD STRUCT
	/*
	// Week 13 - now we can buffer all models
	int numObjs = objs.size();
	for (int i = 0; i < numObjs; i++)
	{
		if (!objs[i].model.buffer(objs[i].texFilename))
			return false;
	}
	*/
	// NEW CLASS
	
	int numObjects = objects.size();
	for (int i = 0; i < numObjects; i++)
	{
		if (objects[i]->shouldRender && !objects[i]->model.buffer(objects[i]->modelFilename))
			return false;
	}
	
	return true;
}

void Engine::getDeltaTime()
{
	// update delta t
	t0 = t;
	t = (float)glfwGetTime();
	dt = t - t0;
	//std::cout << "dt = " << dt << "\n\n";
}

void Engine::updateSimTime()
{
	sim_t0 = sim_t;
	sim_t = t - pause_t;
}
