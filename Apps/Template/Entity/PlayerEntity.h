#pragma once

#include "Ledlib2d/Entity.h"

using namespace Ledlib;

class PlayerEntity : public Entity {
public:
	PlayerEntity();
	void OnStart();
	void OnUpdate();
	void OnRender();
	void OnEnd();
};

