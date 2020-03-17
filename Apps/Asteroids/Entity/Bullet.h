#pragma once

#include "Ledlib2d/Entity.h"

using namespace Ledlib;

class Bullet : public Entity {
public:
	glm::vec3 velocity;
	float countdown;

	void OnStart();
	void OnUpdate();
	void OnRender();
	void OnExit();
};
