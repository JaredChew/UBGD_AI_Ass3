#pragma once

#include "../particleAffector.h"

class DefaultParticleEffect : public ParticleAffector {

public:

	DefaultParticleEffect();
	~DefaultParticleEffect();

	void update();

	void setColourAffector(const glm::vec4 &colourAffector);

	void setRotateAffector(const glm::vec3 &rotateAffector);
	void setSizeAffector(const glm::vec2 &sizeAffector);

	void setSpeedAffector(const glm::vec3 &speedAffector);
	void setDirectionAffector(const glm::vec3 &directionAffector);
	void setAccelerationAffector(const glm::vec3 &accelerationAffector);

	void setSpawnAffector(const glm::vec3 &spawnAffector);

	glm::vec4 getColourAffector();

	glm::vec3 getRotateAffector();
	glm::vec2 getSizeAffector();

	glm::vec3 getSpeedAffector();
	glm::vec3 getDirectionAffector();
	glm::vec3 getAccelerationAffector();

	glm::vec3 getSpawnAffector();

};