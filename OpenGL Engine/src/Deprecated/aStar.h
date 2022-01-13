#pragma once

#include <iostream>
#include <vector>
#include <utility> 
#include <queue>

#include "PathNode.h"

#include "../Data Types/bitData.h"

#define NORTH_EAST 0
#define SOUTH_EAST 1
#define SOUTH_WEST 2
#define NORTH_WEST 3

#define HORIVERTI_COST 10
#define DIAGONAL_COST 14

class A_Star {

private:

	std::vector< std::vector<PathNode> > &grid;

private:

	int tilesWidth;
	int tilesHeight;

	int startNodeX;
	int startNodeY;

	int endNodeX;
	int endNodeY;

	bool pathFound;
	bool allowDiagonal;
	bool allowCrossCorners;

	bool isCornerPossible[4];
	//BitData8 isCornerPossible;

	std::pair<int, int> nextPath;

	std::vector<std::pair<int, int>> toCheck;
	std::vector<std::pair<int, int>> searchList;

	std::vector<std::pair<int, int>> path; //stack alternative? - if dont want history

	std::queue<std::pair<int, int>> previouslySearched;

private:

	int calculateNodeTraversal(int x, int y);

	bool isAtDestination();
	bool possiblePassthrough(const int& x, const int& y);

	void getNextPath(const int& x, const int& y);

	void searchPath();
	void evaluateNextPath();

	void updateSearchList();
	void updateCrossCorner(const int& direction);
	void updateNeighbourNode(const int& x, const int& y);

	void setCost();
	void setParent(const int& x, const int& y);
	void setPathToDestination(const int& x, const int& y);

public:

	A_Star(std::vector< std::vector<PathNode> > &grid, PathNode& start, PathNode& end, const bool& allowDiagonal, const int& allowCrossCorners);
	~A_Star();

	//void reset();

	void searchCotinuous();
	void searchStepByStep();

	bool isPathFound();

	std::vector<std::pair<int, int>> getPath();
	std::queue<std::pair<int, int>>& getPreviouslySearched();

};