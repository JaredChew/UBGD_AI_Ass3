#include "customParticleEffect.h"

CustomParticleEffect_Type1::CustomParticleEffect_Type1(const bool &accelerationEffect, const bool &colourEffect, const bool &sizeEffect, const bool &rotateEffect, const bool &speedEffect) {

	colourAffector = glm::vec4(0.0f);

	rotateAffector = glm::vec3(0.0f);
	sizeAffector = glm::vec2(0.0f);

	speedAffector = glm::vec3(1.0f);
	directionAffector = glm::vec3(0.0f, 1.0f, 0.0f);
	accelerationAffector = glm::vec3(0.0f);

	spawnAffector = glm::vec3(0.0f);

	colourTarget = glm::vec4(0.0f);
	accelerationTarget = glm::vec3(0.0f);
	rotateTarget = glm::vec3(0.0f);
	sizeTarget = glm::vec2(0.0f);

	spawnShape = SpawnShape::POINT;
	directionShape = DirectionShape::BLAST;

	accelerationType = EffectType::FLAT;
	colourType = EffectType::FLAT;
	sizeType = EffectType::FLAT;
	rotateType = EffectType::FLAT;

	effectTime = 1.0f;
	effectTimeCounter = 0.0f;

	this->accelerationEffect = accelerationEffect;
	this->colourEffect = colourEffect;
	this->sizeEffect = sizeEffect;
	this->rotateEffect = rotateEffect;
	this->speedEffect = speedEffect;

	updateEffectTime = accelerationEffect | colourEffect | sizeEffect | rotateEffect | speedEffect;

}

CustomParticleEffect_Type1::~CustomParticleEffect_Type1() { }

void CustomParticleEffect_Type1::applyEffect(const glm::vec4 &affector, glm::vec4 &toEffect, const EffectType &effectType) {

	switch (effectType) {

		case EffectType::ASCENDING:
			toEffect.x = Math::interpolate(toEffect.x, affector.x, effectTimeCounter, effectTime);
			toEffect.y = Math::interpolate(toEffect.x, affector.x, effectTimeCounter, effectTime);
			break;

		case EffectType::DESCENDING:
			toEffect.x = Math::interpolate(affector.x, toEffect.x, effectTimeCounter, effectTime);
			toEffect.y = Math::interpolate(affector.y, toEffect.y, effectTimeCounter, effectTime);
			break;

		case EffectType::CYCLE:
			toEffect += affector * sin(glm::length(toEffect));
			break;

		case EffectType::FLAT:
		default:
			toEffect = affector;
			break;

	}

	updateEffectTime = true;

}

void CustomParticleEffect_Type1::applyEffect(const glm::vec3 &affector, glm::vec3 &toEffect, const EffectType &effectType) {

	switch (effectType) {

		case EffectType::ASCENDING:
			toEffect.x = Math::interpolate(toEffect.x, affector.x, effectTimeCounter, effectTime);
			toEffect.y = Math::interpolate(toEffect.x, affector.x, effectTimeCounter, effectTime);
			break;

		case EffectType::DESCENDING:
			toEffect.x = Math::interpolate(affector.x, toEffect.x, effectTimeCounter, effectTime);
			toEffect.y = Math::interpolate(affector.y, toEffect.y, effectTimeCounter, effectTime);
			break;

		case EffectType::CYCLE:
			toEffect += affector * sin(glm::length(toEffect));
			break;

		case EffectType::FLAT:
		default:
			toEffect = affector;
			break;

	}

	updateEffectTime = true;

}

void CustomParticleEffect_Type1::applyEffect(const glm::vec2 &affector, glm::vec2 &toEffect, const EffectType &effectType) {

	switch (effectType) {

		case EffectType::ASCENDING:
			toEffect.x = Math::interpolate(toEffect.x, affector.x, effectTimeCounter, effectTime);
			toEffect.y = Math::interpolate(toEffect.x, affector.x, effectTimeCounter, effectTime);
			break;

		case EffectType::DESCENDING:
			toEffect.x = Math::interpolate(affector.x, toEffect.x, effectTimeCounter, effectTime);
			toEffect.y = Math::interpolate(affector.y, toEffect.y, effectTimeCounter, effectTime);
			break;

		case EffectType::CYCLE:
			toEffect += affector * sin((float)Window::getTimeElapsed_Seconds());
			break;

		case EffectType::FLAT:
		default:
			toEffect = affector;
			break;

	}

	updateEffectTime = true;

}

void CustomParticleEffect_Type1::produceSpawnLocation() {

	switch (spawnShape) {

		case SpawnShape::POINT:
			spawnAffector = glm::vec3(0.0f);
			break;

		case SpawnShape::SQUARE:
			spawnAffector = glm::vec3(
				Utilities::Random::randBetweenFloat(-1.0f, 1.0f),
				Utilities::Random::randBetweenFloat(-1.0f, 1.0f),
				Utilities::Random::randBetweenFloat(-1.0f, 1.0f)
			);
			break;

		case SpawnShape::TRIANGLE:
			spawnAffector = glm::vec3(
				Utilities::Random::randBetweenFloat(-0.3f, 0.3f),
				Utilities::Random::randBetweenFloat(0.0f, 1.0f),
				Utilities::Random::randBetweenFloat(-0.3f, 0.3f)
			);
			break;

		case SpawnShape::CIRCLE:

			do {

				spawnAffector = glm::vec3(Utilities::Random::randBetweenFloat(-1.0f, 1.0f),
										  Utilities::Random::randBetweenFloat(-1.0f, 1.0f),
										  Utilities::Random::randBetweenFloat(-1.0f, 1.0f)
								);

			} while (abs(glm::length(spawnAffector)) > 1.0f);

			break;

	}

}

void CustomParticleEffect_Type1::produceDirection() {

	switch (directionShape) {

		case DirectionShape::CONCENTRATED:
			directionAffector = glm::vec3(0.0f, 1.0f, 0.0f);
			break;

		case DirectionShape::SPREAD:
			directionAffector = glm::vec3(
				Utilities::Random::randBetweenFloat(-0.3f, 0.3f),
				1.0f,
				Utilities::Random::randBetweenFloat(-0.3f, 0.3f)
			);
			break;

		case DirectionShape::BLAST:
			directionAffector = glm::vec3(
				Utilities::Random::randBetweenFloat(-1.0f, 1.0f),
				Utilities::Random::randBetweenFloat(-1.0f, 1.0f),
				Utilities::Random::randBetweenFloat(-1.0f, 1.0f)
			);
			break;

	}

}

glm::vec3 CustomParticleEffect_Type1::produceSpeed() {

	return glm::vec3(
		Utilities::Random::randBetweenFloat(speedAffector.x, speedAffector.y),
		Utilities::Random::randBetweenFloat(speedAffector.x, speedAffector.y),
		Utilities::Random::randBetweenFloat(speedAffector.x, speedAffector.y)
	);

}

void CustomParticleEffect_Type1::update() {

	if (accelerationEffect) { applyEffect(accelerationTarget, accelerationAffector, accelerationType); }
	if (colourEffect) { applyEffect(colourTarget, colourAffector, colourType); }
	if (sizeEffect) { applyEffect(sizeTarget, sizeAffector, sizeType); }
	if (rotateEffect) { applyEffect(rotateTarget, rotateAffector, rotateType); }

	if (updateEffectTime) {

		effectTimeCounter += Window::getDeltaTime_Seconds();

		if (effectTimeCounter >= effectTime) { effectTimeCounter = 0.0f; }

	}

}

void CustomParticleEffect_Type1::setSpawnShape(const SpawnShape &spawnShape) {
	this->spawnShape = spawnShape;
}

void CustomParticleEffect_Type1::setDirectionShape(const DirectionShape &directionShape) {
	this->directionShape = directionShape;
}

void CustomParticleEffect_Type1::setColourTarget(const glm::vec4 &colourTarget) {
	this->colourTarget = colourTarget;
}

void CustomParticleEffect_Type1::setRotateTarget(const glm::vec3 &rotateTarget) {
	this->rotateTarget = rotateTarget;
}

void CustomParticleEffect_Type1::setSizeTarget(const glm::vec2 &sizeTarget) {
	this->sizeTarget = sizeTarget;
}

void CustomParticleEffect_Type1::setAccelerationTarget(const glm::vec3 &accelerationTarget) {
	this->accelerationTarget = accelerationTarget;
}

void CustomParticleEffect_Type1::setSpeedRange(const glm::vec2 &speedAffector) {
	this->speedAffector = glm::vec3(speedAffector.x, speedAffector.y, 0.0f);
}

void CustomParticleEffect_Type1::setAccelerationType(const EffectType &accelerationType) {
	this->accelerationType = accelerationType;
}

void CustomParticleEffect_Type1::setColourType(const EffectType &colourType) {
	this->colourType = colourType;
}

void CustomParticleEffect_Type1::setSizeType(const EffectType &sizeType) {
	this->sizeType = sizeType;
}

void CustomParticleEffect_Type1::setRotateType(const EffectType &rotateType) {
	this->rotateType = rotateType;
}

void CustomParticleEffect_Type1::setEffectTime(const float &effectTime) {
	this->effectTime = effectTime;
}

glm::vec4 CustomParticleEffect_Type1::getColourTarget() {
	return colourTarget;
}

glm::vec2 CustomParticleEffect_Type1::getSizeTarget() {
	return sizeTarget;
}

glm::vec3 CustomParticleEffect_Type1::getAccelerationTarget() {
	return accelerationTarget;
}

glm::vec3 CustomParticleEffect_Type1::getRotateTarget() {
	return rotateTarget;
}

glm::vec4 CustomParticleEffect_Type1::getColourAffector() {
	return colourEffect ? colourAffector : glm::vec4(0.0f);
}

glm::vec3 CustomParticleEffect_Type1::getRotateAffector() {
	return rotateEffect ? rotateAffector : glm::vec3(0.0f);
}

glm::vec2 CustomParticleEffect_Type1::getSizeAffector() {
	return sizeEffect ? sizeAffector : glm::vec3(0.0f);
}

glm::vec3 CustomParticleEffect_Type1::getAccelerationAffector() {
	return accelerationEffect ? accelerationAffector: glm::vec3(0.0f);
}

glm::vec3 CustomParticleEffect_Type1::getSpeedAffector() {
	return speedEffect ? produceSpeed() : speedAffector ;
}

glm::vec3 CustomParticleEffect_Type1::getDirectionAffector() {

	produceDirection();

	return directionAffector;

}

glm::vec3 CustomParticleEffect_Type1::getSpawnAffector() {

	produceSpawnLocation();

	return spawnAffector;

}

glm::vec3 CustomParticleEffect_Type1::getSpeedRange() {
	return speedAffector;
}

bool CustomParticleEffect_Type1::getAceelerationEffect() {
	return accelerationEffect;
}

bool CustomParticleEffect_Type1::getColourEffect() {
	return colourEffect;
}

bool CustomParticleEffect_Type1::getSizeEffect() {
	return sizeEffect;
}

bool CustomParticleEffect_Type1::getRotateEffect() {
	return rotateEffect;
}

bool CustomParticleEffect_Type1::getSpeedEffect() {
	return speedEffect;
}

float CustomParticleEffect_Type1::getEffectTime() {
	return effectTime;
}

float CustomParticleEffect_Type1::getEffectTimeCounter() {
	return effectTimeCounter;
}

CustomParticleEffect_Type1::EffectType CustomParticleEffect_Type1::getAccelerationType() {
	return accelerationType;
}

CustomParticleEffect_Type1::EffectType CustomParticleEffect_Type1::getColourType() {
	return colourType;
}

CustomParticleEffect_Type1::EffectType CustomParticleEffect_Type1::getSizeType() {
	return sizeType;
}

CustomParticleEffect_Type1::EffectType CustomParticleEffect_Type1::getRotateType() {
	return rotateType;
}
