#pragma once

#include "Ledlib2d/Scene/Entity.h"

using namespace Ledlib;

class TestEntity : public Entity {
public:
	float hue;
	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnRender();
};
