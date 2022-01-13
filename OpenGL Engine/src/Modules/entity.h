#pragma once

#include "transform.h"
#include "boundary.h"
#include "canvas.h"
#include "physics.h"

class Entity {

private:
	friend class Canvas;

public:
	Transform transform;

	Boundary *boundary; //Collision* collision;
	Physics *physics;

public:
	Entity(const bool &collision = false, const bool &physical = false);
	virtual ~Entity();

};