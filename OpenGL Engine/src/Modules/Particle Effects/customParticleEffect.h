#pragma once

#include <glm/geometric.hpp>

#include "../particleAffector.h"

#include "../../Math/generalMath.h"

#include "../../Utilities/random.h"

#include "../../window.h"

class CustomParticleEffect_Type1 : public ParticleAffector {

public:

	enum class SpawnShape { POINT, CIRCLE, TRIANGLE, SQUARE };
	enum class DirectionShape { BLAST, SPREAD, CONCENTRATED };
	enum class EffectType { FLAT, ASCENDING, DESCENDING, CYCLE };

private:

	SpawnShape spawnShape;
	DirectionShape directionShape;

	EffectType accelerationType;
	EffectType colourType;
	EffectType sizeType;
	EffectType rotateType;

	glm::vec4 colourTarget;
	glm::vec3 accelerationTarget;
	glm::vec3 rotateTarget;
	glm::vec2 sizeTarget;

	bool accelerationEffect;
	bool colourEffect;
	bool sizeEffect;
	bool rotateEffect;
	bool speedEffect;
	bool updateEffectTime;

	float effectTime;
	float effectTimeCounter;

private:

	void applyEffect(const glm::vec4 &affector, glm::vec4 &toEffect, const EffectType &effectType);
	void applyEffect(const glm::vec3 &affector, glm::vec3 &toEffect, const EffectType &effectType);
	void applyEffect(const glm::vec2 &affector, glm::vec2 &toEffect, const EffectType &effectType);

	void produceSpawnLocation();
	void produceDirection();

	glm::vec3 produceSpeed();

public:

	CustomParticleEffect_Type1(const bool &accelerationEffect, const bool &colourEffect, const bool &sizeEffect, const bool &rotateEffect, const bool &speedEffect);
	~CustomParticleEffect_Type1();

	void update();

	void setSpeedRange(const glm::vec2 &speedAffector);

	void setSpawnShape(const SpawnShape &spawnShape);
	void setDirectionShape(const DirectionShape &directionShape);

	void setColourTarget(const glm::vec4 &colourTarget);
	void setRotateTarget(const glm::vec3 &rotateTarget);
	void setSizeTarget(const glm::vec2 &sizeTarget);
	void setAccelerationTarget(const glm::vec3 &accelerationTarget);

	void setAccelerationType(const EffectType &accelerationType);
	void setColourType(const EffectType &colourType);
	void setSizeType(const EffectType &sizeType);
	void setRotateType(const EffectType &rotateType);

	void setEffectTime(const float &effectTime);

	glm::vec4 getColourAffector();
	glm::vec2 getSizeAffector();
	glm::vec3 getAccelerationAffector();
	glm::vec3 getRotateAffector();

	glm::vec4 getColourTarget();
	glm::vec2 getSizeTarget();
	glm::vec3 getAccelerationTarget();
	glm::vec3 getRotateTarget();

	glm::vec3 getSpeedAffector();
	glm::vec3 getDirectionAffector();
	glm::vec3 getSpawnAffector();

	glm::vec3 getSpeedRange();

	bool getAceelerationEffect();
	bool getColourEffect();
	bool getSizeEffect();
	bool getRotateEffect();
	bool getSpeedEffect();

	float getEffectTime();
	float getEffectTimeCounter();

	EffectType getAccelerationType();
	EffectType getColourType();
	EffectType getSizeType();
	EffectType getRotateType();

};