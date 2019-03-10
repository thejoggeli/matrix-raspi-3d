#pragma once

#include "Ledlib2d/Entity.h"

using namespace Ledlib;

class TestEntity : public Entity {
public:
	float hue;
	float angle;
	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnCollision(const CollisionData& data);
};
