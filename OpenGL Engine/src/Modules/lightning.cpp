#include "lightning.h"

Lightning::Lightning(const float &length, const float &height, const float &width, const int &layerSplit) { //const float& height, const float& length, const float& width, const int& layerSplit

	this->length = length;
	this->height = height;
	this->width = width;

	this->layerSplit = layerSplit;

	init();

}

Lightning::~Lightning() {

	glDeleteTextures(1, &textureID);

}

void Lightning::init() {

	int currentLayer = 0;

	std::vector<float> xPosition;
	//std::vector<float> yPosition;

	//splitLightning(xPosition, yPosition, startX, startX + length, startY, currentLayer); //new test
	splitLightningHorizontal(xPosition, length, 0, currentLayer); //old

	std::sort(xPosition.begin(), xPosition.end());
	//std::sort(yPosition.begin(), yPosition.end());

	float previousPosY = 0;
	float nextPosY = 0;

	do {

		previousPosY = Utilities::Random::randBetweenFloat(-height, height); //Utilities::randAboveFloat(true); //randAboveFloat(-height, height, true);
		nextPosY = Utilities::Random::randBetweenFloat(-height, height); //Utilities::randAboveFloat(true); //randAboveFloat(-height, height, true);

	}
	while (previousPosY == nextPosY);

	for (int i = 0; i < xPosition.size() - 1; i++) {

		//first triangle

		lightningVertices.push_back(0.0f + xPosition[i]);
		lightningVertices.push_back((0.0f + previousPosY) + width); //old lightning split
		//lightningVertices.push_back(yPosition[i] + width); //new lightning Split test
		lightningVertices.push_back(0.0f);

		lightningTexture.push_back(0);
		lightningTexture.push_back(0);

		lightningVertices.push_back(0.0f + xPosition[i + 1]);
		lightningVertices.push_back((0.0f + nextPosY) + width); //old lightning split
		//lightningVertices.push_back(yPosition[i + 1] + width); //new lightning Split test
		lightningVertices.push_back(0.0f);

		lightningTexture.push_back(0);
		lightningTexture.push_back(1);

		lightningVertices.push_back(0.0f + xPosition[i + 1]);
		lightningVertices.push_back((0.0f + nextPosY) - width); //old lightning split
		//lightningVertices.push_back(yPosition[i + 1] - width); //new lightning Split test
		lightningVertices.push_back(0.0f);

		lightningTexture.push_back(1);
		lightningTexture.push_back(1);

		//second triangle

		lightningVertices.push_back(0.0f + xPosition[i + 1]);
		lightningVertices.push_back((0.0f + nextPosY) - width); //old lightning split
		//lightningVertices.push_back(yPosition[i + 1] - width); //new lightning Split test
		lightningVertices.push_back(0.0f);

		lightningTexture.push_back(1);
		lightningTexture.push_back(1);

		lightningVertices.push_back(0.0f + xPosition[i]);
		lightningVertices.push_back((0.0f + previousPosY) - width); //old lightning split
		//lightningVertices.push_back(yPosition[i] - width); //new lightning Split test
		lightningVertices.push_back(0.0f);

		lightningTexture.push_back(1);
		lightningTexture.push_back(0);

		lightningVertices.push_back(0.0f + xPosition[i]);
		lightningVertices.push_back((0.0f + previousPosY) + width); //old lightning split
		//lightningVertices.push_back(yPosition[i] + width); //new lightning Split test
		lightningVertices.push_back(0.0f);

		lightningTexture.push_back(0);
		lightningTexture.push_back(0);

		previousPosY = nextPosY;

		do {

			nextPosY = Utilities::Random::randBetweenFloat(-height, height); //Utilities::randAboveFloat(true); //randAboveFloat(-height, height, true);

		}
		while (previousPosY == nextPosY);

	}

}

//new test
void Lightning::splitLightning(std::vector<float> &xPosition, std::vector<float> &yPosition, const float &startX, const float &endX, const float &startY, int &currentLayer) {

	const float gradiant = Math::StraightLineEquation::gradiantFromTwoPoint(startX, startY, endX, height);

	const float perpendicularGradiant = Math::StraightLineEquation::perpendicularGradiant(gradiant);

	const float intersection = Math::StraightLineEquation::intersectingPoint(startY, endX, perpendicularGradiant);

	const float randomPosY = Utilities::Random::randBetweenFloat(-height, height);

	const float midpoint = Math::StraightLineEquation::xPosAtYPos(randomPosY, perpendicularGradiant, intersection);

	yPosition.push_back(randomPosY);
	xPosition.push_back(midpoint);

	currentLayer++;

	if (currentLayer >= layerSplit) { return; }

	splitLightning(xPosition, yPosition, startX, midpoint, randomPosY, currentLayer); //left

	splitLightning(xPosition, yPosition, midpoint, endX, randomPosY, currentLayer); //right

}

//Old
void Lightning::splitLightningHorizontal(std::vector<float> &xPosition, const float &length, const float &startX, int &currentLayer) { //std::vector<float>& xPosition, const float& length, const float& startX, const int& maxLayers, int& currentLayer

	const float midpoint = startX + (length / 2.f);

	currentLayer++;

	xPosition.push_back(midpoint);

	if (currentLayer > layerSplit) {
		return;
	}

	splitLightningHorizontal(xPosition, length - (length - midpoint), midpoint, currentLayer); //right

	splitLightningHorizontal(xPosition, length - (length - midpoint), startX, currentLayer); //left

}

void Lightning::generateLightning(const float &affectWidth, const float &affectHeight) { //const float& height, const float& width

	float previousPosY = 0;
	float nextPosY = 0;

	do {

		previousPosY = Utilities::Random::randBetweenFloat(-height, height); //Utilities::randAboveFloat(true); //randAboveFloat(-height, height, true);
		nextPosY = Utilities::Random::randBetweenFloat(-height, height); //Utilities::randAboveFloat(true); //randAboveFloat(-height, height, true);

	}
	while (previousPosY == nextPosY);

	for (int i = 1, j = 0; i < lightningVertices.size(); i += 3, j++) {

		float firstPointX = 0;
		float secondPointX = 0;

		float firstPointY = 0;
		float secondPointY = 0;

		if (j > 5) {

			j = 0;

			previousPosY = nextPosY;

			do {

				nextPosY = Utilities::Random::randBetweenFloat((-height - affectHeight), (affectHeight + height)); //Utilities::randAboveFloat(true); //randAboveFloat(-height, height, true);

			}
			while (nextPosY == previousPosY);

		}

		switch (j) {

			case 0: //FT - top left

				firstPointX = lightningVertices[i - 1];
				firstPointY = lightningVertices[i];

				//Drop down

			case 5: //ST - top left

				lightningVertices[i] = previousPosY + (affectWidth + width);

				break;

			case 1: //FT - top right

				secondPointX = lightningVertices[i - 1];
				secondPointY = lightningVertices[i];

				lightningVertices[i] = nextPosY + (affectWidth + width);

				break;

			case 2: //FT - bottom right
			case 3: //ST - bottom right

				lightningVertices[i] = nextPosY - (affectWidth + width);

				break;

			case 4: //ST - bottom left

				lightningVertices[i] = previousPosY - (affectWidth + width);

				break;

		}

	}
}

void Lightning::draw(const float &affectWidth, const float &affectHeight) { //const float& length, const float& height, const float& width, const int& layerSplit

	generateLightning(affectWidth, affectHeight);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	/*
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, lightningVertices.data());
	glTexCoordPointer(2, GL_FLOAT, 0, lightningTexture.data());

	glDrawArrays(GL_TRIANGLES, 0, lightningVertices.size() / 3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	*/
}