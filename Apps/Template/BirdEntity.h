#pragma once

#include "Ledlib2d/Entity.h"

using namespace Ledlib;

class BirdEntity : public Entity {
public:
	BirdEntity();

	void OnStart();
	void OnUpdate();
	void OnRender();
};

