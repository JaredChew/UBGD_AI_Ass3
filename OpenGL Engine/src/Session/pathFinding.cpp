#include "pathFinding.h"

PathFinding_Demo::PathFinding_Demo(Window *const window, Keyboard *const kbd, Mouse *const mse, Camera *const camera) : wnd(wnd), kbd(kbd), mse(mse), camera(camera) {

	searchType = PathFinder::SearchType::A_STAR; //DIJKSTRA //A_STAR

	//djktra = nullptr;
	//astr = nullptr;
	pathFind = nullptr;

	allowDiagonal = true;
	allowCrossCorners = true;

	displaySearchProgress = true;

	processRunning = false;

	//grid = std::vector < std::vector<Node> >(GRID_WIDTH, std::vector<Node>(GRID_HEIGHT));

	editState = EditState::START;

	startX = 0;
	startY = 0;

	endX = 1;
	endY = 1;

	mouseToGridX = 0;
	mouseToGridY = 0;

	nodeWeight = 0;

	//Init frame buffer
	glGenFramebuffers(1, &frameBuffer);

	//Init textures
	OpenGL::initTexture(cellGridTextureID, 1, GRID_WIDTH, GRID_HEIGHT);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cellGridTextureID, 0);

	Shader::initTextureColour(glm::mat4(1.0f), 1.0f, 1.0f, 1.0f); //new float[3]{ 0.0f, 0.0f, 0.0f }
	Geometry::drawSquare(cellGridTextureID);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	initGrid();

	std::cout << "\nControls:\nPress P to search path\nPress X to reset grid\nPress 1-5 to switch between START, END, WALL, WEIGHT UP, WEIGHT DOWN in order" << "\n";
	std::cout << "Press Q to switch searh types (Dijkstra / A Star)\nPress W to display searching paths\nPress E toggle diagonal path\nPress R to toggle cross corners" << "\n";
	std::cout << "Press A to increase all Node's weight by " << WEIGHT_CHANGE_DEFAULT << "\nPress S to decrease all Node's weight by " << WEIGHT_CHANGE_DEFAULT << "\n";
	std::cout << "\nCurrent Edit state: " << editState + 1 << "\nCurrent search type: " << (int)searchType << "\n";
	std::cout << "Allow diagonal: " << allowDiagonal << "\nAllow cross corner: " << (allowDiagonal && allowCrossCorners ? 1 : 0) << "\nDisplay search progress: " << displaySearchProgress << "\n";
	std::cout << "\nNote: Cross corners is only toggled when diagonal search is allowed" << "\n";

}

PathFinding_Demo::~PathFinding_Demo() {

	resetGrid();

	//delete djktra;
	//djktra = nullptr;

	//delete astr;
	//astr = nullptr;

	delete pathFind;
	pathFind = nullptr;

	thread.clear();

	glUseProgram(0);

	glDeleteTextures(1, &cellGridTextureID);

	glDeleteFramebuffers(1, &frameBuffer);

}

void PathFinding_Demo::initGrid() {

	for (int x = 0; x < GRID_WIDTH; x++) {

		std::vector<PathNode> row;

		for (int y = 0; y < GRID_HEIGHT; y++) {

			row.push_back(PathNode(x, y, 1));

		}

		grid.push_back(row);

	}

}

void PathFinding_Demo::resetGridStatus() {

	for (int x = 0; x < GRID_WIDTH; x++) {

		for (int y = 0; y < GRID_HEIGHT; y++) {

			grid[x][y].resetStatus();

		}

	}

	startX = 0;
	startY = 0;

	endX = 1;
	endY = 1;

}

void PathFinding_Demo::reweightGrid() {

	for (int x = 0; x < GRID_WIDTH; x++) {

		for (int y = 0; y < GRID_HEIGHT; y++) {

			grid[x][y].setWeight(grid[x][y].getWeight() + nodeWeight);

		}

	}

}

void PathFinding_Demo::resetGrid() {

	grid.clear();
	//grid.resize(GRID_WIDTH, std::vector<Node>(GRID_HEIGHT));

}

void PathFinding_Demo::processEdit(const int &posX, const int &posY) {

	switch (editState) {

		case EditState::START:

			OpenGL::editTexel(cellGridTextureID, startX, startY, 255, 255, 255);
			OpenGL::editTexel(cellGridTextureID, posX, posY, 0, 230, 0);

			startX = posX;
			startY = posY;

			break;

		case EditState::END:

			OpenGL::editTexel(cellGridTextureID, endX, endY, 255, 255, 255);
			OpenGL::editTexel(cellGridTextureID, posX, posY, 230, 0, 0);

			endX = posX;
			endY = posY;

			break;

		case EditState::CLEAR:

			OpenGL::editTexel(cellGridTextureID, posX, posY, 255, 255, 255);

			grid[posX][posY].setOpenNode(true);

			break;

		case EditState::WALL:

			OpenGL::editTexel(cellGridTextureID, posX, posY, 0, 0, 255);

			grid[posX][posY].setOpenNode(false);

			break;

		case EditState::WEIGHT_UP:
		case EditState::WEIGHT_DOWN:

			int weight = grid[posX][posY].getWeight() + (editState == EditState::WEIGHT_DOWN ? -10 : 10);

			grid[posX][posY].setWeight(weight);

			int weight_abs = abs(weight);

			int r = weight < 0 ? weight_abs : 100;
			int g = weight_abs % 100;
			int b = weight >= 0 ? weight : 100;

			OpenGL::editTexel(cellGridTextureID, posX, posY, r, g, b);

			break;

	}

}

void PathFinding_Demo::controls() {

	if (kbd->isPressedOnce('P')) {

		std::cout << "\nProcessing " << ((int)searchType ? "A Star" : "Dijkstra") << "\n";

		processRunning = true;

		if (nodeWeight) { reweightGrid(); }

		//djktra = new Dijkstra(grid, PathNode(startX, startY), PathNode(endX, endY));
		//astr = new A_Star(grid, PathNode(startX, startY), PathNode(endX, endY), allowDiagonal, allowCrossCorners);
		pathFind = new PathFinder(grid, PathNode(startX, startY), PathNode(endX, endY), allowDiagonal, allowCrossCorners, searchType);

		timer.recordTick();

	}

	if (kbd->isPressedOnce('X')) {

		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

		if (OpenGL::checkIsFramebufferReady()) {

			glBindTexture(GL_TEXTURE_2D, 0);
			glViewport(0, 0, wnd->getWidth(), wnd->getHeight());

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cellGridTextureID, 0);

			Shader::initTextureColour(glm::mat4(1.0f), 1.0f, 1.0f, 1.0f); //new float[3]{ 0.0f, 0.0f, 0.0f }
			Geometry::drawSquare(cellGridTextureID);

		}

		resetGrid();
		initGrid();

		std::cout << "\nGrid reseted" << "\n";

	}

	for (int i = 0; i < EditState::SIZE; ++i) {

		if (kbd->isPressedOnce(GLFW_KEY_1 + i)) {

			editState = (EditState)i;

			std::cout << "\nEdit state changed to: " << editState + 1 << "\n";

		}

	}

	if (mse->isPressed(MOUSE_BUTTON_LEFT)) {

		mouseToGridX = (int)(((double)GRID_WIDTH / (double)WINDOW_WIDTH) * mse->getPosition().x);
		mouseToGridY = GRID_HEIGHT - (int)(((double)GRID_HEIGHT / (double)WINDOW_HEIGHT) * mse->getPosition().y) - 0.5;

		if (mouseToGridX < 0 || mouseToGridX >= GRID_WIDTH) { return; }
		if (mouseToGridY < 0 || mouseToGridY >= GRID_HEIGHT) { return; }

		processEdit(mouseToGridX, mouseToGridY);

	}

	if (kbd->isPressedOnce('Q')) {

		searchType = (int)searchType ? PathFinder::SearchType::DIJKSTRA : PathFinder::SearchType::A_STAR;

		std::cout << "\nCurrent search method changed to: " << (int)searchType << "\n";

	}

	if (kbd->isPressedOnce('W')) {

		displaySearchProgress = !displaySearchProgress;

		std::cout << "\nDisplay search progress changed to: " << displaySearchProgress << "\n";

	}

	if (kbd->isPressedOnce('E')) {

		allowDiagonal = !allowDiagonal;

		std::cout << "\nDiagonal path changed to: " << allowDiagonal << "\n";

	}

	if (kbd->isPressedOnce('R')) {

		allowCrossCorners = !allowCrossCorners;

		std::cout << "\nCross corners changed to: " << (allowDiagonal & allowCrossCorners) << "\n";

	}

	if (kbd->isPressedOnce('A')) {

		nodeWeight += WEIGHT_CHANGE_DEFAULT;

		std::cout << "\nAll node's weight will be increased by: " << nodeWeight << "\n";

	}

	if (kbd->isPressedOnce('S')) {

		nodeWeight -= WEIGHT_CHANGE_DEFAULT;

		std::cout << "\nAll node's weight will be decreased by: " << nodeWeight << "\n";

	}

}
/*
void PathFinding_Demo::renderSearch_Dijsktra() {

	while (!djktra->getPreviouslySearched().empty()) {

		std::pair<int, int> posXY = djktra->getPreviouslySearched().front();

		OpenGL::editTexel(cellGridTextureID, posXY.first, posXY.second, 100, 100, 100);

		djktra->getPreviouslySearched().pop();

	}

	//Multithreading
	//if (!thread.empty()) { thread.clear(); }

	//GLuint& cellGridTextureID_local = cellGridTextureID;

	//for (int i = 0; i < djktra->getPreviouslySearched().size(); ++i) {

	//	std::pair<int, int> posXY = djktra->getPreviouslySearched().front();
	//	thread.push_back(std::thread(

	//		[&cellGridTextureID_local, &posXY](const GLuint &cellGridTextureID, const std::pair<int, int>& posXY) {
	//
	//			//printf("\nID: %d | X: %i | Y: %d", cellGridTextureID, posXY.first, posXY.second);
	//			OpenGL::editTexel(cellGridTextureID, posXY.first, posXY.second, 100, 100, 100);

	//		}

	//	, std::ref(cellGridTextureID_local), std::ref(posXY)));

	//	if (thread[i].joinable()) { thread[i].join(); }
	//	//thread[i].detach();

	//	OpenGL::editTexel(cellGridTextureID, posXY.first, posXY.second, 100, 100, 100);

	//	djktra->getPreviouslySearched().pop();

	//}

	OpenGL::editTexel(cellGridTextureID, startX, startY, 0, 255, 0);

}

void PathFinding_Demo::renderSearch_aStar() {

	while (!astr->getPreviouslySearched().empty()) {

		std::pair<int, int> posXY = astr->getPreviouslySearched().front();

		OpenGL::editTexel(cellGridTextureID, posXY.first, posXY.second, 100, 100, 100);

		astr->getPreviouslySearched().pop();

	}

	OpenGL::editTexel(cellGridTextureID, startX, startY, 0, 255, 0);

}

void PathFinding_Demo::renderPath_Dijsktra() {

	std::vector<std::pair<int, int>> path = djktra->getPath();

	if (path.empty()) { return; }

	for (int i = 1; i < path.size() - 1; ++i) {

		OpenGL::editTexel(cellGridTextureID, path[i].first, path[i].second, 255, 255, 0);

	}

}

void PathFinding_Demo::renderPath_aStar() {

	std::vector<std::pair<int, int>> path = astr->getPath();

	if (path.empty()) { return; }

	for (int i = 1; i < path.size() - 1; ++i) {

		OpenGL::editTexel(cellGridTextureID, path[i].first, path[i].second, 255, 255, 0);

	}

}

void PathFinding_Demo::processDijkstra() {

	if (displaySearchProgress) { djktra->searchStepByStep(); }
	else { djktra->searchCotinuous(); }

	if (djktra->isPathFound()) {

		timer.recordTock();
		printf("Completed. Time taken: %f\n", timer.getDeltaTimeSeconds());

		renderPath_Dijsktra();

		processRunning = false;

		delete djktra; djktra = nullptr; resetGridStatus();

		return;

	}

	if (displaySearchProgress) { renderSearch_Dijsktra(); }

}

void PathFinding_Demo::processAstar() {

	if (displaySearchProgress) { astr->searchStepByStep(); }
	else { astr->searchCotinuous(); }

	if (astr->isPathFound()) {

		timer.recordTock();
		printf("\nPath found. Time taken: %f", timer.getDeltaTimeSeconds());

		renderPath_aStar();

		processRunning = false;

		delete astr; astr = nullptr; resetGridStatus();

		return;

	}

	if (displaySearchProgress) { renderSearch_aStar(); }

}

void PathFinding_Demo::runSimulation() {

	switch (searchType) {

	case DIJKSTRA:
		processDijkstra();
		break;

	case A_STAR:
		processAstar();
		break;

	}

}
*/

void PathFinding_Demo::processPathFinder() {

	if (displaySearchProgress) { pathFind->searchStepByStep(); }
	else { pathFind->searchCotinuous(); }

	if (pathFind->getIsPathFound()) {

		timer.recordTock();
		printf("\nPath found. Time taken: %f", timer.getDeltaTimeSeconds());

		std::vector<std::pair<int, int>> path = pathFind->getPath();

		if (path.empty()) { return; }

		for (int i = 1; i < path.size() - 1; ++i) {

			OpenGL::editTexel(cellGridTextureID, path[i].first, path[i].second, 255, 255, 0);

		}

		processRunning = false;

		delete pathFind; pathFind = nullptr; resetGridStatus();

		return;

	}

	if (displaySearchProgress) {

		while (!pathFind->getPreviouslySearched().empty()) {

			std::pair<int, int> posXY = pathFind->getPreviouslySearched().front();

			OpenGL::editTexel(cellGridTextureID, posXY.first, posXY.second, 100, 100, 100);

			pathFind->getPreviouslySearched().pop();

		}

		OpenGL::editTexel(cellGridTextureID, startX, startY, 0, 255, 0);

	}

}

void PathFinding_Demo::update() {

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (OpenGL::checkIsFramebufferReady()) {

		glBindTexture(GL_TEXTURE_2D, 0);
		glViewport(0, 0, wnd->getWidth(), wnd->getHeight());

		Shader::defaultDraw(glm::mat4(1.0f));
		Geometry::drawSquare(cellGridTextureID);

	}

	if (processRunning) { /*runSimulation();*/processPathFinder(); }
	else { controls(); }

}