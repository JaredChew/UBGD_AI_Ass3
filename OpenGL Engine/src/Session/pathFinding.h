#pragma once

#include <chrono>
#include <vector>
#include <thread>

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

#include "../Modules/pathNode.h"
//#include "../Modules/dijkstra.h"
//#include "../Modules/aStar.h"
#include "../Modules/pathFinder.h"

#include "../Utilities/timer.h"

#define GRID_WIDTH 70 //11 //960
#define GRID_HEIGHT 40 //6 //540

#define WEIGHT_CHANGE_DEFAULT 10

class PathFinding_Demo : public Session {

private:

	Window *const wnd;
	Keyboard *const kbd;
	Mouse *const mse;
	Camera *const camera;

private:

	enum EditState { START, END, WALL, CLEAR, WEIGHT_UP, WEIGHT_DOWN, SIZE = 6 } editState;

	GLuint frameBuffer;

	GLuint cellGridTextureID;

	std::vector< std::vector<PathNode> > grid;

	int startX;
	int startY;

	int endX;
	int endY;

	int mouseToGridX;
	int mouseToGridY;

	int nodeWeight;

	//Dijkstra* djktra;
	//A_Star* astr;
	PathFinder *pathFind;

	bool processRunning;
	bool displaySearchProgress;
	bool allowDiagonal;
	bool allowCrossCorners;

	PathFinder::SearchType searchType;

	std::vector<std::thread> thread;

	Timer timer;

private:

	void processEdit(const int &posX, const int &posY);

	void controls();

	void initGrid();
	void resetGrid();
	void resetGridStatus();
	void reweightGrid();
	/*
	void renderSearch_Dijsktra();
	void renderPath_Dijsktra();

	void renderSearch_aStar();
	void renderPath_aStar();

	void processDijkstra();
	void processAstar();
	*/
	void processPathFinder();

	void runSimulation();

public:

	PathFinding_Demo(Window *const window, Keyboard *const kbd, Mouse *const mse, Camera *const camera);
	~PathFinding_Demo();

	void preUpdate() override { };
	void update() override;
	void postUpdate() override { };

	void render() override { };

};