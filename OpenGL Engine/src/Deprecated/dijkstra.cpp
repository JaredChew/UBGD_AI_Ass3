#include "dijkstra.h"

Dijkstra::Dijkstra(std::vector< std::vector<PathNode> >& grid, PathNode& start, PathNode& end) : grid(grid) {

	tilesWidth = grid.size();
	tilesHeight = grid[0].size();

	startNodeX = start.getX();
	startNodeY = start.getY();

	endNodeX = end.getX();
	endNodeY = end.getY();

	nodesVisited = 0;
	totalOperation = 0;

	direction = NORTH;

	pathFound = false;

	currentSearch.emplace(startNodeX, startNodeY);

}

Dijkstra::~Dijkstra() { }

void Dijkstra::searchCotinuous() {

	while (!pathFound) {

		searchPath();

		if (nodesVisited >= (tilesWidth * tilesHeight)) { break; pathFound = true; }

	}

}

void Dijkstra::searchStepByStep() {

	if (pathFound) { return; }

	searchPath();

}

void Dijkstra::setParent(const int& x, const int& y) {

	switch (direction) {

	case NORTH:
		grid[x][y].setParent(&grid[x][y + 1]);
		break;

	case SOUTH:
		grid[x][y].setParent(&grid[x][y - 1]);
		break;

	case EAST:
		grid[x][y].setParent(&grid[x - 1][y]);
		break;

	case WEST:
		grid[x][y].setParent(&grid[x + 1][y]);
		break;

	}

}

bool Dijkstra::possiblePassthrough(const int& x, const int& y) {

	if (x < 0 || x >= tilesWidth) { return false; }
	if (y < 0 || y >= tilesHeight) { return false; }

	if (!grid[x][y].isNodeOpen()) { return false; }
	if (!grid[x][y].isOrphan()) { return false; }

	return true;

}

void Dijkstra::setPath(const int& x, const int& y) {

	grid[x][y].setVisited(true);

	path.push_back(std::make_pair(x, y));

	if (grid[x][y].getX() != startNodeX || grid[x][y].getY() != startNodeY) {
		setPath(grid[x][y].getParent()->getX(), grid[x][y].getParent()->getY());
	}

}

void Dijkstra::processNode(const int& x, const int& y) {

	previouslySearched.emplace(x, y);

	++nodesVisited;

	grid[x][y].setOpenNode(false);

	//setParent(x, y);

	if (x == endNodeX && y == endNodeY) {

		pathFound = true;

		setPath(x, y);

		return;

	}

	nextSearch.emplace(x, y);

}

void Dijkstra::resetSearchCycle() {

	direction = NORTH;

	currentSearch.pop();

	if (currentSearch.empty()) { currentSearch.swap(nextSearch); nextSearch = { }; }

}

void Dijkstra::searchPath() {

	previouslySearched = { };

	if (currentSearch.empty()) { pathFound = true; } //fail safe

	for (int i = 0; i < 4; ++i) {

		int x = currentSearch.front().first, y = currentSearch.front().second;

		switch (direction) {

			case NORTH:
				if (possiblePassthrough(x, y - 1)) { grid[x][y - 1].setParent(&grid[x][y]); processNode(x, y - 1); }
				break;

			case SOUTH:
				if (possiblePassthrough(x, y + 1)) { grid[x][y + 1].setParent(&grid[x][y]); processNode(x, y + 1); }
				break;

			case EAST:
				if (possiblePassthrough(x + 1, y)) { grid[x + 1][y].setParent(&grid[x][y]); processNode(x + 1, y); }
				break;

			case WEST:
				if (possiblePassthrough(x - 1, y)) { grid[x - 1][y].setParent(&grid[x][y]); processNode(x - 1, y); }
				break;

		}

		++direction;
		++totalOperation;

	}
	
	resetSearchCycle();

}

int Dijkstra::getTotalPathSteps() {
	return path.size();
}

int Dijkstra::getTotalOperations() {
	return totalOperation;
}

std::queue<std::pair<int, int>>& Dijkstra::getPreviouslySearched() {
	return previouslySearched;
}

std::vector<std::pair<int, int>> Dijkstra::getPath() {
	return path;
}

bool Dijkstra::isPathFound() {
	return pathFound;
}