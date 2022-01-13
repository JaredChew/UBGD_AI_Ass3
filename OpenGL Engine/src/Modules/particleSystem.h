#pragma once

#include <array>

#include "sprite.h"
#include "particleEntity.h"
#include "particleAffector.h"

#include "Particle Effects/defaultParticleEffect.h"

#include "../Utilities/random.h"

class ParticleSystem {

private:

	ParticleEntity *particle;

private:

	ParticleEntity **particlesEmitted;

	ParticleAffector *particleAffector;

	//int maxParticlesType;
	//int numParticlesAlive;
	int maxParticlesEmittable;

	float emittionDelay;

	double emittionCooldown;

	bool isEmittionEnable;

public:

	Transform transform;

public:

	ParticleSystem(ParticleEntity *const particle, const int &maxParticlesEmittable, const float &emittionDelay, const bool &isEmittionEnable = true);
	~ParticleSystem();

	//void setParticles(ParticleObject *const particles);
	void setEmittionDelay(const float &emittionDelay);
	void setIsEmittionEnable(const bool &isEmittionEnable);
	void setMaxParticlesEmittable(const int &maxParticlesEmittable);
	void setParticleAffector(ParticleAffector *const particleAffector);

	void update();
	void render();

	//ParticleObject getParticle(const int &index);
	ParticleEntity getEmittedParticle(const int &index);

	int numberOfParticlesAlive();

	int getMaxParticlesEmittable();
	float getEmittionDelay();
	bool getIsEmittionEnable();

};

/*

void ParticleSystem::setParticles(ParticleObject *const particles) {

	if (emitParticles && numParticlesAlive) { return; }

	delete[] particlesEmitted;
	particlesEmitted = nullptr;

	delete[] this->particles;
	this->particles = nullptr;

	this->particles = particles;

	maxParticlesType = sizeof(*particles) / sizeof(particles[0]);

}

*/