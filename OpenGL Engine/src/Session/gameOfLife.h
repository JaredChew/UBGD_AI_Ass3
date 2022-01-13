#pragma once

#include <chrono>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../window.h"
#include "../keyboard.h"
#include "../mouse.h"
#include "../camera.h"
#include "../opengl.h"
#include "../geometry.h"
#include "../defaultSettings.h"
#include "../session.h"
#include "../shader.h"

#define TOTAL_CELLGRID_TEXTURE 2

#define CELLGRID_TEXTURE_PROCESS 0
#define CELLGRID_TEXTURE_DISPLAY 1

#define GRID_MAX_WIDTH_DEFAULT 5000 //13040
#define GRID_MAX_HEIGHT_DEFAULT 5000 //13040

#define DEFAULT_VALUE_CHANGE 10.0f

class GameOfLife_Demo : public Session {

private:

	Window *const wnd;
	Keyboard *const kbd;
	Mouse *const mse;
	Camera *const camera;

private:

	glm::mat4 mvpMatrix;
	glm::mat4 modelMatrix;

	GLuint frameBuffer;

	GLuint shaderProgram_GameOfLifeAI;
	GLuint shaderProgram_MouseCellManipulate;

	GLuint cellGridTextureID[TOTAL_CELLGRID_TEXTURE];

	std::chrono::steady_clock::time_point tick;
	std::chrono::steady_clock::time_point tock;

	std::chrono::nanoseconds deltaTime;

	float objectSize;

	float resizeWidth;
	float resizeHeight;

	int cellGridWidth;
	int cellGridHeight;

	int cursorPositionX;
	int cursorPositionY;

	int cursorPrevPositionX;
	int cursorPrevPositionY;

	unsigned long long int updateSpeed;

	bool autoPlay;
	bool inResizeMenu;

private:

	void gameOfLifeAI(const GLuint &texture);

	void copyToRender();
	void mouseManipulateCellState(const GLuint &texture);
	void updateSimulationState();
	void runSimulation();

	void resizeCellTexture(const float &width, const float &height);
	void resizeMenu();

	void cursorControls();
	void controls();

public:

	GameOfLife_Demo(Window *const window, Keyboard *const kbd, Mouse *const mse, Camera *const camera);
	~GameOfLife_Demo();

	void preUpdate() override { };
	void update() override;
	void postUpdate() override { };

	void render() { };

};