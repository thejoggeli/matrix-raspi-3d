#pragma once

#include "Ledlib2d/Entity.h"

using namespace Ledlib;

class Wall : public Entity {
public:
	std::vector<glm::vec3> vertices;
	void OnStart();
	void OnUpdate();
	void OnRender();
	void OnEnd();
	void AddVertex(float x, float y);
	void Build();

};

