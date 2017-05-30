#include "Player.h"
#include "Engine.h"

Player::Player()
{
	up = true;
	collider = Collider::Sphere;
	bulletTimer = 0;
}

Player::Player(glm::vec3 loc, glm::vec3 vel, glm::vec3 force, float mass, glm::vec3 rot, glm::vec3 scal, bool render)
	: Object(loc, vel, force, mass, rot, scal, render)
{
	up = true;
	collider = Collider::Sphere;
	bulletTimer = 0;

	invul = false;
	invulDuration = 3000;
	bulletLifespan = 400;
	health = 5;
	invulTimer = 0;
}

Player::~Player()
{
}

void Player::move()
{
	if (invul) beInvul(); // play out invulnerability for duration
	airCheck(); // moves player through jump or makes player land from jump
	validateInput(); // checks if user can shoot
	manageBullets(); // checks collisions w/bullet and enemy

	// DEBUG
	/*std::cout <<
		"Player\nUp? = " << up <<
		
		"\nForce = (" << transform.rigidBody.force.x <<
		", " << transform.rigidBody.force.y <<
		", " << transform.rigidBody.force.z <<
		
		"\nVel = (" << transform.rigidBody.vel.x <<
		", " << transform.rigidBody.vel.y <<
		", " << transform.rigidBody.vel.z <<
		
		"\nLoc = (" << transform.location.x <<
		", " << transform.location.y <<
		", " << transform.location.z << "\n-\n";*/
}

void Player::beInvul()
{
	// increment invul timer
	//invulTimer++;
	invulTimer += 2 * invulDuration * Engine::dt;

	//std::cout << "invul timer = " << invulTimer << "\n";

	// if time's up, return to being vulnerable
	if (invulTimer >= invulDuration)
	{
		invulTimer = 0;
		invul = false;
		std::cout << "vulnerable!\n";
	}
}

void Player::airCheck()
{
	/*
	// check if grounded
	if (up && transform.location.y <= 0.0f)
	{
		// fix y position, set y velocity to zero, and set up = false
		transform.location.y = 0.0f;
		transform.rigidBody.vel.y = 0.0f;
		up = false;
	}

	// apply gravity ONLY WHEN IN THE AIR
	if (up)
		transform.rigidBody.force.y = -1.0f;
	*/

	if (up)
	{
		// check if grounded
		if (transform.location.y <= 0.0f)
		{
			// fix y position, set y velocity to zero, and set up = false
			transform.location.y = 0.0f;
			transform.rigidBody.vel.y = 0.0f;
			transform.rigidBody.force.y = 0.0f;
			up = false;
		}
		// otherwise, assume airborn and apply gravity
		else
		{
			transform.rigidBody.force.y = -24.0f;
			if (transform.rigidBody.vel.y < 0)
			{
				transform.rigidBody.force.y *= 2.2f;
			}
		}
	}
}

void Player::validateInput()
{
	// time to get user input!
	// get current window, needed to validate input
	GLFWwindow* window = glfwGetCurrentContext();
	
	// keep player in bounds
	keepInBounds();

	// wasd to move
	// A = left, D = right
	// note: walking is done via delta v (used to be delta position)
	//transform.rigidBody.vel.x = 0.0f;
	capVel();

	// update previous states and get current states
	for (int i = 0; i < 2; i++)
		prevKeyStates[i] = currKeyStates[i];		

	currKeyStates[0] = glfwGetKey(window, GLFW_KEY_A);
	currKeyStates[1] = glfwGetKey(window, GLFW_KEY_D);

	if (currKeyStates[0] == GLFW_PRESS)
	{
		transform.rigidBody.vel.x += -0.02f;
		//transform.location.x += -0.01f;
	}
	
	if (currKeyStates[1] == GLFW_PRESS)
	{
		transform.rigidBody.vel.x += 0.02f;
		//transform.location.x += 0.01f;
	}	

	// apply friction if no left/right keys pressed, player has nonzero velocity, and player is grounded
	applyFriction(window);
	
	// K = jump, it's a delta v applied directly to the transform
	if (!up && glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		transform.rigidBody.vel.y = 15.0f;
		up = true;
	}

	// J = shoot
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && canFire)
		shoot();
}

void Player::keepInBounds()
{
	// keep player from exiting left and right bounds of game scene
	if (transform.location.x <= 0.0f)// || transform.location.x >= 30.0f)	
	{
		transform.location.x = 0.0f;
		transform.rigidBody.vel.x = 0.0f;
	}
	else if (transform.location.x >= 30.0f)
	{
		transform.location.x = 0.0f;
		scorePoint();
	}
}


void Player::applyFriction(GLFWwindow* window)
{
	if ((currKeyStates[0] != GLFW_PRESS)
		&& (currKeyStates[1] != GLFW_PRESS)
		&& transform.rigidBody.vel.x != 0.0f
		&& !up)
	{
		// store current friction value in a temp
		float frictionWithDir = friction;

		// friction temp is currently positive,
		// so make it negative only if x velocity is positive
		// if
		if (transform.rigidBody.vel.x > 0.0f)
			frictionWithDir = -frictionWithDir;
		
		// DEBUG
		//std::cout << "old vel = " << transform.rigidBody.vel.x << "\n";

		// apply friction to net force
		transform.rigidBody.vel.x += frictionWithDir;

		// DEBUG
		//std::cout << "friction with dir = " << frictionWithDir <<
		//	"\nnew vel = " << transform.rigidBody.vel.x << "\n-\n";
	}
}

// cap vel at + or - maxVel, depending on direction of vel
void Player::capVel()
{
	if (glm::abs(transform.rigidBody.vel.x) >= maxVel)
		if (transform.rigidBody.vel.x > 0)
			transform.rigidBody.vel.x = maxVel;
		else
			transform.rigidBody.vel.x = -maxVel;
}

void Player::shoot()
{
	// OLD WAY - EXPENSIVE!!!!!!
	/*
	// declare and initialize a new Bullet object, then push that to objects vector
	Bullet* b = new Bullet(transform.location, 3.0f);
	b->modelFilename = "models/teapot.obj";
	b->model.buffer(b->modelFilename);
	Engine::objects.push_back(b);
	*/

	// NEW WAY
	//if (numBullets < maxBullets)
	//{
		// get proper index of bullet in objects vector
		// index 0 = camera
		// index 1 = player
		// index 2 = bullet
		// index 3 = enemy
	int index = 2;
		/*
		switch (numBullets)
		{
		case 0: index = 3; break;
		case 1: index = 4; break;
		case 2: index = 5; break;
		default: break;
		}
		*/
	// reposition bullet to current position
	Engine::objects[index]->transform.location = transform.location;
		
	// set its speed and allow it to move
	((Bullet*)Engine::objects[index])->shouldMove = true;
	((Bullet*)Engine::objects[index])->xSpeed = 12.0f;

		// increment numBullets
		//numBullets++;

		// DEBUG
		//std::cout <<
		//	"Fire!" <<
		//	"\nindex = " << index <<
		//	"\nPosition = (" <<
		//		Engine::objects[index]->transform.location.x <<
		//		", " << Engine::objects[index]->transform.location.y <<
		//		", " << Engine::objects[index]->transform.location.z << ")" <<
		//	"\nShould move = " << ((Bullet*)Engine::objects[index])->shouldMove <<
		//	"\nxSpeed = " << ((Bullet*)Engine::objects[index])->xSpeed <<
		//	"\nxVel = " << (Engine::objects[index])->transform.rigidBody.vel.x << "\n";


		// disable firing so player doesn't fire on every pressed frame
		canFire = false;
	//}
}

void Player::manageBullets()
{
	// check if bullet should move
	Bullet* b = (Bullet*)Engine::objects[2];
	if (b->shouldMove)
	{
		// then it will move (already taken care of when xSpeed set)
		// increment timer
		//bulletTimer++;
		bulletTimer += 1000 * Engine::dt;
		//std::cout << "bt = " << bulletTimer << "\n\n";

		// and check if bullet lifespan has been lived out
		if (bulletTimer >= bulletLifespan)
			removeBullet(b);
		

		// FINALS WEEK: manually check collisions between bullet and enemy
		if (b->collidesWith(*Engine::objects[3]) && Engine::objects[3]->health > 0)
		{
			// remove bullet and damage enemy
			std::cout << "Enemy is hit!\n";
			removeBullet(b);
			Engine::objects[3]->hit();
		}

		// DEBUG
		//std::cout <<
		//	"Bullet" <<
		//	"\nPosition = (" << b->transform.location.x <<
		//	", " << b->transform.location.y <<
		//	", " << b->transform.location.z <<
		//	"\n-\n";
	}
}

void Player::removeBullet(Bullet* b)
{

	// ...put bullet offscreen
	b->transform.location = glm::vec3(-100, -100, -100);
	
	// set its move to false
	b->shouldMove = false;
	
	// decrement numBullets
	//numBullets--;

	// enable player to fire again
	canFire = true;

	// reset timer
	bulletTimer = 0;
}

void Player::die()
{
	std::cout << "you died ahahahahaha\n";

	// close the window - game over
	glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
}

void Player::scorePoint()
{
	// increment score
	score++;

	// check if player has scored enough points to win
	if (score >= scoreToWin)
	{
		// cap score
		score = scoreToWin;

		// DEBUG
		std::cout << "you win!!\n";

		// move enemy off screen and out of player's way (de facto, remove enemy)
		Engine::objects[3]->transform.location = glm::vec3(-200, 2000, 2000);

		// close window - game over (the good kind)
		glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
	}
	// DEBUG
	std::cout << "Point scored! score = " << score << "\n";
}
