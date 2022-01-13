#include "canvas.h"

Canvas::Canvas() { }

Canvas::~Canvas() { }

void Canvas::detectCollision() {

	for (std::list<Entity>::iterator i = entities.begin(); i != entities.end(); ++i) {

		for (std::list<Entity>::iterator j = entities.begin(); j != entities.end(); ++j) {


			if (i != j) { i->boundary->isColliding(*(j->boundary)); }

		}

	}

}

void Canvas::applyPhysics() {

	//

}

void Canvas::update() {

	//detectCollision();
	//applyPhysics();

}

void Canvas::addEntity(const Entity &entity) {

	entities.push_back(entity);

}
