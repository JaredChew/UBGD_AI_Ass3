#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>

#include "../openGL.h"

class Wave {

private:

	std::vector<GLfloat> waveVertices;
	std::vector<GLfloat> waveTexture;

	GLuint textureID;

	int textureCoverage;

	int waveWidth;
	int waveHeight;

	float sizeWidth;
	float sizeHeight;

	float heightFrequency;
	float speed;

	float radian;

private:

	void init();

public:

	Wave(const int &waveWidth, const int &waveHeight,
		 const float &sizeWidth, const float &sizeHeight,
		 const float &heightFrequency, const float &speed);

	~Wave();

	void initTexture(const char *directiory, const int &textureCoverage);

	void setHeightFrequency(const float &heightFrequency);
	void setSpeed(const float &speed);

	void draw(const float &affectHeightFrequency = 0, const float &affectSpeed = 0);

};