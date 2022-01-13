#pragma once

#include <vector>
#include <algorithm>

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "../Utilities/random.h"

#include "../Math/straightLineEquation.h"

class Lightning {

private:

	std::vector<GLfloat> lightningVertices;
	std::vector<GLfloat> lightningTexture;

	GLuint textureID;

	float length;
	float height;
	float width;

	int layerSplit;

private:

	void init();
	//void initTexture();

	void splitLightning(std::vector<float> &xPosition, std::vector<float> &yPosition, const float &startX, const float &endX, const float &startY, int &currentLayer); //new testing
	void splitLightningHorizontal(std::vector<float> &xPosition, const float &length, const float &startX, int &currentLayer); //old

	void generateLightning(const float &affectWidth, const float &affectHeight);

public:

	Lightning(const float &length, const float &height, const float &width, const int &layerSplit);
	~Lightning();

	void draw(const float &affectWidth = 0, const float &affectHeight = 0);

};