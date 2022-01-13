#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../window.h"
#include "../keyboard.h"
#include "../mouse.h"
#include "../camera.h"
#include "../opengl.h"
#include "../session.h"
#include "../shader.h"

#include "../Modules/sprite.h"
#include "../Modules/canvas.h"
#include "../Modules/entity.h"
#include "../Modules/boidEntity.h"
#include "../Modules/flockingBehaviour.h"

#include "../Utilities/random.h"

#define DEFAULT_VALUE_CHANGE 1.0f

#define MAX_FISH 100
#define DEFAULT_FISH_TOTAL 100

#define TOTAL_OBSTACLES 3

#define DEFAULT_SEPERATION_DISTANCE 4.0f
#define DEFAULT_ALIGNMENT_DISTANCE 5.0f
#define DEFAULT_COHESION_DISTANCE 5.0f

#define DEFAULT_SEPERATION_WEIGHT 7.0f
#define DEFAULT_ALIGNMENT_WEIGHT 5.0f
#define DEFAULT_COHESION_WEIGHT 3.0f

#define DEFAULT_ATTRACTION_DISTANCE 7.0f
#define DEFAULT_AVOIDANCE_DISTANCE 10.0f

#define DEFAULT_ATTRACTION_WEIGHT 5.0f
#define DEFAULT_AVOIDANCE_WEIGHT 30.0f

#define FISH_VELOCITY_LIMIT 50.0f
#define FISH_ACCELERATION_LIMIT 5.0f
#define FISH_SWIMMABLE_DISTANCE 40.0f

#define BAIT_SPEED 50.0f
#define SHARK_SPEED 30.0f

// *** BAIT *** //

class Bait : public Entity {

private:

	Keyboard *const kbd;

public:

	Sprite *sprite;

	float movementSpeed;

public:

	Bait(Window *const wnd, Keyboard *const kbd, Camera *const camera, GLuint *const targetRender) : Entity(true), kbd(kbd) {

		sprite = new Sprite(wnd, camera, "../Media/particle.bmp", targetRender);

		sprite->setBlendColour(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
		sprite->setBlendType(Sprite::BlendType::BLEND_MULTIPLY, false);

		movementSpeed = BAIT_SPEED;

		transform.setPosition(
			glm::vec3(Utilities::Random::randBetweenFloat(-10.0f, 10.0f),
			Utilities::Random::randBetweenFloat(-10.0f, 10.0f),
			0.0f)
		);

	}

	virtual ~Bait() {

		delete sprite;
		sprite = nullptr;

	}

	void controls() {

		float movementSpeed = this->movementSpeed * (float)Window::getDeltaTime_Seconds();

		if (kbd->isPressed('W')) { transform.translate(glm::vec3(0.0f, movementSpeed * 2.0f, 0.0f)); }
		if (kbd->isPressed('A')) { transform.translate(glm::vec3(-movementSpeed, 0.0f, 0.0f)); }
		if (kbd->isPressed('S')) { transform.translate(glm::vec3(0.0f, -movementSpeed * 2.0f, 0.0f)); }
		if (kbd->isPressed('D')) { transform.translate(glm::vec3(movementSpeed, 0.0f, 0.0f)); }

	}

	void update() {

		controls();

	}

};

// *** SHARK *** //

class Shark : public Entity {

private:

	Keyboard *const kbd;

public:

	Sprite *sprite;

	float movementSpeed;

public:

	Shark(Window *const wnd, Keyboard *const kbd, Camera *const camera, GLuint *const targetRender) : Entity(true), kbd(kbd) {

		sprite = new Sprite(wnd, camera, "../Media/shark.bmp", targetRender);

		movementSpeed = SHARK_SPEED;

		transform.setDimensionLocal(glm::vec3(1.5f, 2.0f, 1.0f));

		transform.setPosition(
			glm::vec3(Utilities::Random::randBetweenFloat(-10.0f, 10.0f),
			Utilities::Random::randBetweenFloat(-10.0f, 10.0f),
			0.0f)
		);

	}

	virtual ~Shark() {

		delete sprite;
		sprite = nullptr;

	}

	void controls() {

		float movementSpeed = this->movementSpeed * (float)Window::getDeltaTime_Seconds();

		if (kbd->isPressed('I')) { transform.translate(glm::vec3(0.0f, movementSpeed * 2.0f, 0.0f)); }
		if (kbd->isPressed('J')) { transform.translate(glm::vec3(-movementSpeed, 0.0f, 0.0f)); }
		if (kbd->isPressed('K')) { transform.translate(glm::vec3(0.0f, -movementSpeed * 2.0f, 0.0f)); }
		if (kbd->isPressed('L')) { transform.translate(glm::vec3(movementSpeed, 0.0f, 0.0f)); }

	}

	void update() {

		controls();

		float rotate = atan2(transform.getPosition().y, transform.getPosition().x) * (float)(180.0f / glm::pi<float>());

		transform.setRotationLocal(glm::vec3(0.0f, 0.0f, rotate));

	}

};

// *** FISH ** //

class Fish_AI : public BoidEntity {

public:

	Sprite *sprite;

public:

	Fish_AI(Window *const wnd, Camera *const camera, GLuint *const targetRender) : BoidEntity() {

		sprite = new Sprite(wnd, camera, "../Media/fish.bmp", targetRender);

		setVelocityLimit(FISH_VELOCITY_LIMIT);
		setAccelerationLimit(FISH_ACCELERATION_LIMIT);

		//transform.setDimensionLocal(glm::vec3(1.3f, 2.2f, 1.0f));

	}

	~Fish_AI() {

		delete sprite;
		sprite = nullptr;

	}

	Transform getTransform() {
		return transform;
	}

};

// *** ROCK ** //

class Rock {

public:

	Sprite *sprite;

	Transform transform;

	//float avoidanceRadius;

public:

	Rock(Window *const wnd, Camera *const camera, GLuint *const targetRender) {

		sprite = new Sprite(wnd, camera, "../Media/rock.bmp", targetRender);

		//transform.setDimensionLocal(glm::vec3(1.3f, 2.2f, 1.0f));

		transform.setPosition(glm::vec3(
			Utilities::Random::randBetweenFloat(-30.0f, 30.0f),
			Utilities::Random::randBetweenFloat(-30.0f, 30.0f),
			0.0f
		));

		transform.setRotationLocal(glm::vec3(
			0.0f,
			0.0f,
			Utilities::Random::randBetweenFloat(-50.0f, 50.0f)
		));

		transform.setDimensionLocal(glm::vec3(
			Utilities::Random::randBetweenFloat(1.0f, 1.5f),
			Utilities::Random::randBetweenFloat(1.0f, 1.5f),
			0.0f
		));

		//avoidanceRadius = glm::length(transform.getDimensionLocal()) * 0.01f;

	}

	~Rock() {

		delete sprite;
		sprite = nullptr;

	}

};

// *** STEERING BEHAVIOUR ** //

class SteeringBehaviour_Demo : public Session {

private:

	Window *const wnd;
	Keyboard *const kbd;
	Mouse *const mse;
	Camera *const camera;

private:

	Canvas canvas;

	Bait *bait;
	Shark *shark;

	Rock **rock;
	Fish_AI **fish;

	FlockingBehaviour *fb;

	GLuint screenTexture;
	GLuint depthBuffer;

	enum class Command { NONE, BOID_CONT, SPRT_DTNC, ALGN_DTNC, CHSN_DTNC, SPRT_WGHT, ALGN_WGHT, CHSN_WGHT, ATTRC_DSNC, AVDN_DTNC } currentCommand;

private:

	void executeCommand(const int &amount);
	void controls();

public:

	SteeringBehaviour_Demo(Window *const wnd, Keyboard *const kbd, Mouse *const mse, Camera *const camera);
	~SteeringBehaviour_Demo();

	void update() override;
	void render();

};