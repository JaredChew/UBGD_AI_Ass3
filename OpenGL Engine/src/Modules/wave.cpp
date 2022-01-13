#include "wave.h"

Wave::Wave(const int &waveWidth, const int &waveHeight, const float &sizeWidth, const float &sizeHeight, const float &heightFrequency, const float &speed) {

	this->waveWidth = waveWidth;
	this->waveHeight = waveHeight;

	this->sizeWidth = sizeWidth;
	this->sizeHeight = sizeHeight;

	this->heightFrequency = heightFrequency;
	this->speed = speed;

	radian = 1.0f;

	textureID = NULL;

	init();

}

Wave::~Wave() {

	glDeleteTextures(1, &textureID);

}

void Wave::init() {

	float texCoord0 = 0;
	float texCoord1 = textureCoverage / 2;

	for (int i = 0; i < waveHeight; i++) {

		for (int j = 0; j < waveWidth; j++) {

			//first triangle

			waveVertices.push_back(0.0f + (j * sizeWidth)); //width - x
			waveVertices.push_back(0.0f); //height - y
			waveVertices.push_back(0.0f + (i * sizeHeight)); //depth - z

			waveTexture.push_back(texCoord0);
			waveTexture.push_back(texCoord1);

			waveVertices.push_back(0.0f + ((j + 1) * sizeWidth)); //width - x
			waveVertices.push_back(0.0f); //height - y
			waveVertices.push_back(0.0f + (i * sizeHeight)); //depth - z

			waveTexture.push_back(texCoord1);
			waveTexture.push_back(texCoord1);

			waveVertices.push_back(0.0f + ((j + 1) * sizeWidth)); //width - x
			waveVertices.push_back(0.0f); //height - y
			waveVertices.push_back(0.0f + ((i + 1) * sizeHeight)); //depth - z

			waveTexture.push_back(texCoord1);
			waveTexture.push_back(texCoord0);

			//second triangle

			waveVertices.push_back(0.0f + ((j + 1) * sizeWidth)); //width - x
			waveVertices.push_back(0.0f); //height - y
			waveVertices.push_back(0.0f + ((i + 1) * sizeHeight)); //depth - z

			waveTexture.push_back(texCoord1);
			waveTexture.push_back(texCoord0);

			waveVertices.push_back(0.0f + (j * sizeWidth)); //width - x
			waveVertices.push_back(0.0f); //height - y
			waveVertices.push_back(0.0f + ((i + 1) * sizeHeight)); //depth - z

			waveTexture.push_back(texCoord0);
			waveTexture.push_back(texCoord0);

			waveVertices.push_back(0.0f + (j * sizeWidth)); //width - x
			waveVertices.push_back(0.0f); //height - y
			waveVertices.push_back(0.0f + (i * sizeHeight)); //depth - z

			waveTexture.push_back(texCoord0);
			waveTexture.push_back(texCoord1);

		}

		texCoord0 += (textureCoverage / 2) / waveWidth;
		texCoord1 += (textureCoverage / 2) / waveHeight;

		if (texCoord0 > textureCoverage / 2) { texCoord0 = 0; }
		if (texCoord1 > textureCoverage) { texCoord1 = textureCoverage / 2; }

	}

}

void Wave::initTexture(const char *directiory, const int &textureCoverage) {

	this->textureCoverage = textureCoverage;

	OpenGL::loadTexture(directiory, textureID);

	float texCoord0 = 0;
	float texCoord1 = textureCoverage / 2;

	for (int i = 0; i < waveHeight; i++) {

		for (int j = 0; j < waveWidth; j++) {

			//first triangle

			waveTexture.push_back(texCoord0);
			waveTexture.push_back(texCoord1);

			waveTexture.push_back(texCoord1);
			waveTexture.push_back(texCoord1);

			waveTexture.push_back(texCoord1);
			waveTexture.push_back(texCoord0);

			//second triangle

			waveTexture.push_back(texCoord1);
			waveTexture.push_back(texCoord0);

			waveTexture.push_back(texCoord0);
			waveTexture.push_back(texCoord0);

			waveTexture.push_back(texCoord0);
			waveTexture.push_back(texCoord1);

		}

		texCoord0 += (textureCoverage / 2) / waveWidth;
		texCoord1 += (textureCoverage / 2) / waveHeight;

		if (texCoord0 > textureCoverage / 2) { texCoord0 = 0; }
		if (texCoord1 > textureCoverage) { texCoord1 = textureCoverage / 2; }

	}

}

void Wave::setHeightFrequency(const float &heightFrequency) {

	this->heightFrequency = heightFrequency;

}

void Wave::setSpeed(const float &speed) {

	this->speed = speed;

}

void Wave::draw(const float &affectHeightFrequency, const float &affectSpeed) { //const float& x, const float& y, const float& z, const int& waveWidth, const int& waveHeight, const float& sizeWidth, const float& sizeHeight, const float& heightFrequency, const float& speed, const int& textureCoverage

	float first = 0;
	float second = (heightFrequency + affectHeightFrequency);

	for (int i = 1, j = 0, widthCounter = 0; i < waveVertices.size(); i += 3, j++) {

		if (j > 5) { j = 0; widthCounter++; first = second; second += (heightFrequency + affectHeightFrequency); }
		if (widthCounter >= waveWidth) { widthCounter = 0; first = 0; second = (heightFrequency + affectHeightFrequency); }

		switch (j) {

			case 0: //FT - top left
			case 4: //ST - bottom left
			case 5: //ST - top left
				waveVertices[i] = 0.0f + sin(radian + first);
				break;

			case 1: //FT - top right
			case 2: //ST - bottom right
			case 3: //ST - bottom right
				waveVertices[i] = 0.0f + sin(radian + second);
				break;

		}

	}

	radian += (speed + affectSpeed);

	if (radian > 360) {
		radian = 0;
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);
	/*
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, waveVertices.data());
	glTexCoordPointer(2, GL_FLOAT, 0, waveTexture.data());
	//glColorPointer(3, GL_UNSIGNED_BYTE, 0, colour.data());

	glDrawArrays(GL_TRIANGLES, 0, waveVertices.size() / 3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_COLOR_ARRAY);
	*/
}