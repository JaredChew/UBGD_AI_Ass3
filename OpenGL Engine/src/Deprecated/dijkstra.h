#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <utility> 
#include <thread>

#include "PathNode.h"

#define NORTH_EAST 0
#define SOUTH_EAST 1
#define SOUTH_WEST 2
#define NORTH_WEST 3

#define NORTH 4
#define EAST 5
#define SOUTH 6
#define WEST 7

class Dijkstra {

private:

	std::vector< std::vector<PathNode> > &grid;

private:

	int tilesWidth;
	int tilesHeight;

	int startNodeX;
	int startNodeY;

	int endNodeX;
	int endNodeY;

	int direction;

	int nodesVisited;
	int totalOperation;

	bool pathFound;

	std::vector<std::pair<int, int>> path;

	std::queue<std::pair<int, int>> nextSearch;
	std::queue<std::pair<int, int>> currentSearch;

	std::queue<std::pair<int, int>> previouslySearched;

private:

	void searchPath();
	void processNode(const int& x, const int& y);

	void resetSearchCycle();

	void setPath(const int& x, const int& y);
	void setParent(const int& x, const int& y);
	//void setNextSearch(const int& x, const int& y);

	bool possiblePassthrough(const int& x, const int& y);

public:

	Dijkstra(std::vector< std::vector<PathNode> > &grid, PathNode& start, PathNode& end);
	~Dijkstra();

	//void reset();

	void searchCotinuous();
	void searchStepByStep();

	int getTotalPathSteps();
	int getTotalOperations();

	bool isPathFound();

	std::queue<std::pair<int, int>> &getPreviouslySearched();
	std::vector<std::pair<int, int>> getPath();

};

/*

void Dijktra::setNextSearch(const int& x, const int& y) {

	for (int i = 0; i < 3; ++i) {

		switch (i) {

			case NORTH:
				if (possiblePassthrough(x, y + 1)) { nextSearch.emplace(x, y - 1); }
				break;

			case SOUTH:
				if (possiblePassthrough(x, y - 1)) { nextSearch.emplace(x, y + 1); }
				break;

			case EAST:
				if (possiblePassthrough(x + 1, y)) { nextSearch.emplace(x + 1, y); }
				break;

			case WEST:
				if (possiblePassthrough(x - 1, y)) { nextSearch.emplace(x - 1, y); }
				break;

		}

	}

}

*/