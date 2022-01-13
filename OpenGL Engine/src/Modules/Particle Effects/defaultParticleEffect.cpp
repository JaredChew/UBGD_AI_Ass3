#include "defaultParticleEffect.h"

DefaultParticleEffect::DefaultParticleEffect() {

	colourAffector = glm::vec4(0.0f);

	rotateAffector = glm::vec3(0.0f);
	sizeAffector = glm::vec2(0.0f);

	speedAffector = glm::vec3(1.0f);
	directionAffector = glm::vec3(0.0f, 1.0f, 0.0f);
	accelerationAffector = glm::vec3(0.0f);

	spawnAffector = glm::vec3(0.0f);

}

DefaultParticleEffect::~DefaultParticleEffect() { }

void DefaultParticleEffect::update() { }

void DefaultParticleEffect::setColourAffector(const glm::vec4 &colourAffector) {
	this->colourAffector = colourAffector;
}

void DefaultParticleEffect::setSpawnAffector(const glm::vec3 &spawnAffector) {
	this->spawnAffector = spawnAffector;
}

void DefaultParticleEffect::setRotateAffector(const glm::vec3 &rotateAffector) {
	this->rotateAffector = rotateAffector;
}

void DefaultParticleEffect::setSizeAffector(const glm::vec2 &sizeAffector) {
	this->sizeAffector = sizeAffector;
}

void DefaultParticleEffect::setSpeedAffector(const glm::vec3 &speedAffector) {
	this->speedAffector = speedAffector;
}

void DefaultParticleEffect::setDirectionAffector(const glm::vec3 &directionAffector) {
	this->directionAffector = directionAffector;
}

void DefaultParticleEffect::setAccelerationAffector(const glm::vec3 &accelerationAffector) {
	this->accelerationAffector = accelerationAffector;
}

glm::vec4 DefaultParticleEffect::getColourAffector() {
	return colourAffector;
}

glm::vec3 DefaultParticleEffect::getRotateAffector() {
	return rotateAffector;
}

glm::vec2 DefaultParticleEffect::getSizeAffector() {
	return sizeAffector;
}

glm::vec3 DefaultParticleEffect::getSpeedAffector() {
	return speedAffector;
}

glm::vec3 DefaultParticleEffect::getDirectionAffector() {
	return directionAffector;
}

glm::vec3 DefaultParticleEffect::getAccelerationAffector() {
	return accelerationAffector;
}

glm::vec3 DefaultParticleEffect::getSpawnAffector() {
	return spawnAffector;
}
