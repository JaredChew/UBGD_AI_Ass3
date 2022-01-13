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
#include "../session.h"

#include "../Modules/canvas.h"
#include "../Modules/entity.h"
#include "../Modules/particleEntity.h"
#include "../Modules/particleSystem.h"

#include "../Modules/Particle Effects/customParticleEffect.h"

#define MAX_PARTICLE_AFFECTORS 3
#define CIRCLE_AFFECTOR 0
#define TRIANGLE_AFFECTOR 1
#define SQUARE_AFFECTOR 2

#define MAX_PARTICLE_SYSTEMS MAX_PARTICLE_AFFECTORS
#define MOVEMENT_SPEED 50.0f
#define MAX_PARTICLE_EMIT 10000
#define EMITTION_DELAY 0.0001f
#define PARTICLE_LIFETIME 5.0f

class ParticlesEffect_Demo : public Session {

private:

	Window *const wnd;
	Keyboard *const kbd;
	Mouse *const mse;
	Camera *const camera;

private:

	GLuint screenTexture;
	GLuint depthBuffer;

	ParticleSystem **particleEmitter;

	int emitterController;

private:

	void controls();

public:

	ParticlesEffect_Demo(Window *const wnd, Keyboard *const kbd, Mouse *const mse, Camera *const camera);
	~ParticlesEffect_Demo();

	void update() override;
	void render();

};