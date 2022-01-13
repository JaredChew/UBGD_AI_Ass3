#pragma once

#include <list>

#include "entity.h"

class Entity;

class Canvas {

private:

	std::list<Entity> entities;

private:

	void detectCollision();
	void applyPhysics();

public:
	Canvas();
	~Canvas();

	void update();

	void addEntity(const Entity &entity);

};