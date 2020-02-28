#pragma once

#include "Ledlib2d/Entity.h"

using namespace Ledlib;

class Goodie : public Entity {
public:
	float radius = 1.0f;
	void OnStart();
	void OnUpdate();
	void OnRender();
	void OnExit();
};

