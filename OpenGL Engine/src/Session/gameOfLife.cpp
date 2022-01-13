#include "gameOfLife.h"

GameOfLife_Demo::GameOfLife_Demo(Window *const window, Keyboard *const kbd, Mouse *const mse, Camera *const camera) : wnd(wnd), kbd(kbd), mse(mse), camera(camera) {

	tick = std::chrono::high_resolution_clock::now();

	deltaTime = std::chrono::nanoseconds(0);

	objectSize = (GRID_MAX_WIDTH_DEFAULT * GRID_MAX_HEIGHT_DEFAULT) * 0.000005;

	updateSpeed = 1000000000;

	cellGridWidth = GRID_MAX_WIDTH_DEFAULT;
	cellGridHeight = GRID_MAX_HEIGHT_DEFAULT;

	resizeWidth = cellGridWidth;
	resizeHeight = cellGridHeight;

	cursorPositionX = GRID_MAX_WIDTH_DEFAULT / 2;
	cursorPositionY = GRID_MAX_HEIGHT_DEFAULT / 2;

	cursorPrevPositionX = cursorPositionX;
	cursorPrevPositionY = cursorPositionY;

	autoPlay = true;
	inResizeMenu = false;

	//Init frame buffer
	glGenFramebuffers(1, &frameBuffer);

	GLuint vertexShader = OpenGL::loadShaderFromFile(GL_VERTEX_SHADER, "../Shaders/default.vert");
	GLuint fragmentShader = OpenGL::loadShaderFromFile(GL_FRAGMENT_SHADER, "../Shaders/gameOfLife.frag");

	if (!OpenGL::initProgramObject_Shader(shaderProgram_GameOfLifeAI, fragmentShader, vertexShader)) {
		Logger::getInstance()->warningLog("Failed to init Game Of Life AI shader program");
	}

	vertexShader = OpenGL::loadShaderFromFile(GL_VERTEX_SHADER, "../Shaders/default.vert");
	fragmentShader = OpenGL::loadShaderFromFile(GL_FRAGMENT_SHADER, "../Shaders/mouseToScreen.frag");

	if (!OpenGL::initProgramObject_Shader(shaderProgram_MouseCellManipulate, fragmentShader, vertexShader)) {
		Logger::getInstance()->warningLog("Failed to init Mouse To Screen shader program");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	//Init textures
	for (int i = 0; i < TOTAL_CELLGRID_TEXTURE; ++i) {

		OpenGL::initTexture(cellGridTextureID[i], 1, GRID_MAX_WIDTH_DEFAULT, GRID_MAX_HEIGHT_DEFAULT);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cellGridTextureID[i], 0);

		Shader::initTextureColour(glm::mat4(1.0f), 0.0f, 0.0f, 0.0f); //new float[3]{ 0.0f, 0.0f, 0.0f }
		Geometry::drawSquare(cellGridTextureID[i]);

	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//init test cell state
	for (int x = 0; x < 10; ++x) {

		OpenGL::editTexel(cellGridTextureID[CELLGRID_TEXTURE_PROCESS], (GRID_MAX_WIDTH_DEFAULT / 2) + x - 4, GRID_MAX_HEIGHT_DEFAULT / 2, 255, 255, 255);
		OpenGL::editTexel(cellGridTextureID[CELLGRID_TEXTURE_DISPLAY], (GRID_MAX_WIDTH_DEFAULT / 2) + x - 4, GRID_MAX_HEIGHT_DEFAULT / 2, 255, 255, 255);

	}

	OpenGL::editTexel(cellGridTextureID[CELLGRID_TEXTURE_DISPLAY], cursorPositionX, cursorPositionY, 191, 255, 255);

	std::cout << "\nControls:\nAll values changes by 10\nPress R to decrease speed\nPress F to increase speed\nPress Q to zoom out\nPress E to zoom in\nPress P to pause/resume auto simulation\nPress X to reset simulation\nPress Z to enter resize menu\nPress N to enter next state when auto run is off" << "\n";
	std::cout << "Press I, J, K, L to move the cursor\nPress W, A, S, D to move the camera\nPress O to kill/spawn a cell at the cursor" << "\n";

}

GameOfLife_Demo::~GameOfLife_Demo() {

	glUseProgram(0);

	glDeleteTextures(TOTAL_CELLGRID_TEXTURE, cellGridTextureID);

	glDeleteFramebuffers(1, &frameBuffer);

}

void GameOfLife_Demo::resizeMenu() {

	if (kbd->isPressedOnce('I')) {

		resizeHeight += DEFAULT_VALUE_CHANGE;

		std::cout << "\nGrid height increased by " << DEFAULT_VALUE_CHANGE << " and is now at " << resizeHeight << "\n";

	}

	if (kbd->isPressedOnce('K')) {

		resizeHeight -= DEFAULT_VALUE_CHANGE;

		std::cout << "\nGrid height decreased by " << DEFAULT_VALUE_CHANGE << " and is now at " << resizeHeight << "\n";

	}

	if (kbd->isPressedOnce('L')) {

		resizeWidth += DEFAULT_VALUE_CHANGE;

		std::cout << "\nGrid width increased by " << DEFAULT_VALUE_CHANGE << " and is now at " << resizeWidth << "\n";

	}

	if (kbd->isPressedOnce('J')) {

		resizeWidth -= DEFAULT_VALUE_CHANGE;

		std::cout << "\nGrid width decreased by " << DEFAULT_VALUE_CHANGE << " and is now at " << resizeWidth << "\n";

	}

	if (kbd->isPressedOnce('O')) {

		resizeCellTexture(resizeWidth, resizeHeight);

		cursorPositionX = resizeWidth / 2;
		cursorPositionY = resizeHeight / 2;

		inResizeMenu = false;

	}

	if (kbd->isPressedOnce('U')) {

		std::cout << "\nExited resize menu" << "\n";

		inResizeMenu = false;

	}

}

void GameOfLife_Demo::cursorControls() {

	if (kbd->isPressed('O')) {
		OpenGL::editTexel(cellGridTextureID[CELLGRID_TEXTURE_PROCESS], cursorPositionX, cursorPositionY, 255, 0, 0);
		OpenGL::editTexel(cellGridTextureID[CELLGRID_TEXTURE_DISPLAY], cursorPositionX, cursorPositionY, 255, 0, 0);
		cursorPrevPositionX = cursorPositionX;
		cursorPrevPositionY = cursorPositionY;
	}

	if (kbd->isPressed('L')) {
		++cursorPositionX;
	}

	if (kbd->isPressed('I')) {
		++cursorPositionY;
	}

	if (kbd->isPressed('J')) {
		--cursorPositionX;
	}

	if (kbd->isPressed('K')) {
		--cursorPositionY;
	}

}

void GameOfLife_Demo::controls() {

	if (kbd->isPressedOnce('P')) {

		autoPlay = !autoPlay;

		std::cout << "\nAuto run simulation is " << (autoPlay ? "on" : "off") << "\n";

	}

	if (kbd->isPressedOnce('N')) {

		if (!autoPlay) {

			runSimulation();

			std::cout << "\nNext simulation state " << "\n";

		}

	}

	if (kbd->isPressedOnce('Z') && !autoPlay) {

		inResizeMenu = true;

		std::cout << "\nResize grid menu controls:\nPress U to quit.\nAll values changes by 10\nPress I to increase height\nPress J to decrease width\nPress K to decrease height\nPress L to increase width\nPress O to apply changes" << "\n";

	}

	if (kbd->isPressedOnce('X') && !autoPlay) {

		//Init texture base colour
		for (int i = 0; i < TOTAL_CELLGRID_TEXTURE; ++i) {

			Shader::initTextureColour(glm::mat4(1.0f), 0.0f, 0.0f, 0.0f); //new float[3]{ 0.0f, 0.0f, 0.0f }
			Geometry::drawSquare(cellGridTextureID[i]);

		}

		cursorPositionX = cellGridWidth / 2;
		cursorPositionY = cellGridHeight / 2;

		std::cout << "\nSimulation reseted" << "\n";

	}

	if (kbd->isPressedOnce('R')) {

		updateSpeed -= DEFAULT_VALUE_CHANGE;

		std::cout << "\nUpdate speed decreased by " << DEFAULT_VALUE_CHANGE << " and now at " << updateSpeed << "ms" << "\n";

	}

	if (kbd->isPressedOnce('F')) {

		updateSpeed += DEFAULT_VALUE_CHANGE;

		std::cout << "\nUpdate speed increased by " << DEFAULT_VALUE_CHANGE << " and now at " << updateSpeed << "ms" << "\n";

	}

	if (kbd->isPressed('Q')) { objectSize -= DEFAULT_VALUE_CHANGE * (objectSize * 0.01f); }
	if (kbd->isPressed('E')) { objectSize += DEFAULT_VALUE_CHANGE * (objectSize * 0.01f); }

}

void GameOfLife_Demo::mouseManipulateCellState(const GLuint &texture) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram_MouseCellManipulate);

	GLuint mousePosXLoc = glGetUniformLocation(shaderProgram_MouseCellManipulate, "mousePosX");

	if (mousePosXLoc != -1) { glUniform1f(mousePosXLoc, (float)mse->getPosition().x); }
	else { Logger::getInstance()->warningLog("SHADER_PROGRAM_MOUSETOSCREEN uniform \"mousePosX\" not found"); }

	GLuint mousePosY_Loc = glGetUniformLocation(shaderProgram_MouseCellManipulate, "mousePosY");

	if (mousePosY_Loc != -1) { glUniform1f(mousePosY_Loc, (float)mse->getPosition().y); }
	else { Logger::getInstance()->warningLog("SHADER_PROGRAM_MOUSETOSCREEN uniform \"mousePosY\" not found"); }

	GLuint mouseisPressedOnce_Loc = glGetUniformLocation(shaderProgram_MouseCellManipulate, "mouseIsPressed");

	if (mouseisPressedOnce_Loc != -1) { glUniform1i(mouseisPressedOnce_Loc, mse->isPressedOnce(MOUSE_BUTTON_LEFT)); }
	else { Logger::getInstance()->warningLog("SHADER_PROGRAM_MOUSETOSCREEN uniform \"mouseIsPressed\" not found"); }

	GLuint sampler2d_Loc = glGetUniformLocation(shaderProgram_MouseCellManipulate, "sampler2d");

	if (sampler2d_Loc != -1) { glUniform1i(sampler2d_Loc, 0); }
	else { Logger::getInstance()->warningLog("SHADER_PROGRAM_MOUSETOSCREEN uniform \"sampler2d\" not found"); }

	GLuint nonNormalisedMatrix_Loc = glGetUniformLocation(shaderProgram_MouseCellManipulate, "nonNormalisedMatrix");

	if (nonNormalisedMatrix_Loc != -1) { glUniformMatrix4fv(nonNormalisedMatrix_Loc, 1, GL_FALSE, glm::value_ptr(mvpMatrix)); }
	else { Logger::getInstance()->warningLog("SHADER_PROGRAM_MOUSETOSCREEN uniform \"nonNormalisedMatrix\" not found"); }

	// reset the mvpMatrix to identity matrix so that it renders fully on texture in normalized device coordinates
	GLuint uMvpMatrix_Loc = glGetUniformLocation(shaderProgram_MouseCellManipulate, "uMvpMatrix");

	if (uMvpMatrix_Loc != -1) { glUniformMatrix4fv(uMvpMatrix_Loc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f))); }
	else { Logger::getInstance()->warningLog("SHADER_PROGRAM_MOUSETOSCREEN uniform \"uMvpMatrix\" not found"); }

	Geometry::drawSquare(texture);

}

void GameOfLife_Demo::gameOfLifeAI(const GLuint &texture) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgram_GameOfLifeAI);

	GLuint gridSizeX_Loc = glGetUniformLocation(shaderProgram_GameOfLifeAI, "gridSizeX");

	if (gridSizeX_Loc != -1) { glUniform1f(gridSizeX_Loc, (float)cellGridWidth); }
	else { Logger::getInstance()->warningLog("SHADER_PROGRAM_GAME_OF_LIFE uniform \"gridSizeX\" not found"); }

	GLuint gridSizeY_Loc = glGetUniformLocation(shaderProgram_GameOfLifeAI, "gridSizeY");

	if (gridSizeY_Loc != -1) { glUniform1f(gridSizeY_Loc, (float)cellGridHeight); }
	else { Logger::getInstance()->warningLog("SHADER_PROGRAM_GAME_OF_LIFE uniform \"gridSizeY\" not found"); }

	GLuint sampler2d_Loc = glGetUniformLocation(shaderProgram_GameOfLifeAI, "sampler2d");

	if (sampler2d_Loc != -1) { glUniform1i(sampler2d_Loc, 0); }
	else { Logger::getInstance()->warningLog("SHADER_PROGRAM_GAME_OF_LIFE uniform \"sampler2d\" not found"); }

	// reset the mvpMatrix to identity matrix so that it renders fully on texture in normalized device coordinates
	GLuint uMvpMatrix_Loc = glGetUniformLocation(shaderProgram_GameOfLifeAI, "uMvpMatrix");

	if (uMvpMatrix_Loc != -1) { glUniformMatrix4fv(uMvpMatrix_Loc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f))); }
	else { Logger::getInstance()->warningLog("SHADER_PROGRAM_GAME_OF_LIFE uniform \"uMvpMatrix\" not found"); }

	Geometry::drawSquare(texture);

}

void GameOfLife_Demo::copyToRender() {

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cellGridTextureID[CELLGRID_TEXTURE_PROCESS], 0);

	Shader::defaultDraw(glm::mat4(1.0f));
	Geometry::drawSquare(cellGridTextureID[CELLGRID_TEXTURE_DISPLAY]);

}

void GameOfLife_Demo::resizeCellTexture(const float &width, const float &height) {

	for (int i = 0; i < TOTAL_CELLGRID_TEXTURE; ++i) {

		Shader::initTextureColour(glm::mat4(1.0f), 0.0f, 0.0f, 0.0f);
		Geometry::drawSquare(cellGridTextureID[i]);

	};

	for (int i = 0; i < TOTAL_CELLGRID_TEXTURE; ++i) {

		//Init textures
		OpenGL::initTexture(cellGridTextureID[i], 1, width, height);

	}

	cellGridWidth = width;
	cellGridHeight = height;

	std::cout << "\nSimulation reseted with new grid size of width " << cellGridWidth << " and height " << cellGridHeight << "\n";

}

void GameOfLife_Demo::updateSimulationState() {

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cellGridTextureID[CELLGRID_TEXTURE_DISPLAY], 0);

	gameOfLifeAI(cellGridTextureID[CELLGRID_TEXTURE_PROCESS]);

	copyToRender();
	/*
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cellGridTextureID[CELLGRID_TEXTURE_DISPLAY], 0);

	mouseManipulateCellState(cellGridTextureID[CELLGRID_TEXTURE_PROCESS]);

	copyToRender();
	*/
}

void GameOfLife_Demo::runSimulation() {

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	if (OpenGL::checkIsFramebufferReady()) {

		glBindTexture(GL_TEXTURE_2D, 0);
		glViewport(0, 0, cellGridWidth, cellGridHeight);

		tock = std::chrono::high_resolution_clock::now();

		deltaTime += std::chrono::duration_cast<std::chrono::nanoseconds>(tock - tick);

		if (deltaTime.count() >= updateSpeed) {

			updateSimulationState();

			deltaTime = std::chrono::nanoseconds(0);

		}

		tick = tock;

	}

}

void GameOfLife_Demo::update() {

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (OpenGL::checkIsFramebufferReady()) {

		glBindTexture(GL_TEXTURE_2D, 0);
		glViewport(0, 0, camera->getRenderWidth(), camera->getRenderHeight());

		modelMatrix = glm::scale<float>(glm::mat4(1.0f), glm::vec3(objectSize, objectSize, 0.0f));

		mvpMatrix = camera->projectionMatrix * camera->getViewMatrix() * modelMatrix;

		Shader::defaultDraw(mvpMatrix);
		Geometry::drawSquare(cellGridTextureID[CELLGRID_TEXTURE_DISPLAY]);

	}

	if (autoPlay) { runSimulation(); }

	if (inResizeMenu) {
		resizeMenu();
	}
	else {
		controls();
		cursorControls();
	}

	if (objectSize < 1.0f) { objectSize = 1.0f; }

	if (cursorPositionX != cursorPrevPositionX || cursorPositionY != cursorPrevPositionY) {

		OpenGL::editTexel(cellGridTextureID[CELLGRID_TEXTURE_DISPLAY], cursorPrevPositionX, cursorPrevPositionY, 0, 0, 0);

		cursorPrevPositionX = cursorPositionX;
		cursorPrevPositionY = cursorPositionY;

	}

	OpenGL::editTexel(cellGridTextureID[CELLGRID_TEXTURE_DISPLAY], cursorPositionX, cursorPositionY, 255, 255, 0);

}