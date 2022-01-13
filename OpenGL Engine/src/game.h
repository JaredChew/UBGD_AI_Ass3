#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>

#include "window.h"
#include "camera.h"
#include "keyboard.h"
#include "mouse.h"
#include "session.h"

#include "Session/gameOfLife.h"
#include "Session/pathFinding.h"
#include "Session/steeringBehaviour.h"
#include "Session/particlesEffect.h"

class Game {

private:

	Window *const wnd;

private:

	Keyboard *kbd;
	Mouse *mse;

	Camera *camera;

	Session *session;

public:
	Game(Window *const wnd);
	~Game();

	void initSessionState();

	void gameLoop();

};