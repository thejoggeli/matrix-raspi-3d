#pragma once

#include "Ledlib2d/Entity.h"

using namespace Ledlib;

class Player : public Entity {
public:
	glm::vec3 velocity;
	glm::vec3 acceleration;
	std::vector<glm::vec2> vertices;

	void OnStart();
	void OnUpdate();
	void OnRender();
	void OnEnd();
};

