#pragma once

#include "Ledlib2d/Entity.h"

class SnakeSegment;

using namespace Ledlib;

class Snake : public Entity {
public:
	std::weak_ptr<SnakeSegment> head;
	void OnStart();
	void OnUpdate();
	void OnRender();
	void OnAfterRender();
	void OnExit();
	std::shared_ptr<SnakeSegment> LastSegment();
	void Grow();
	void OnCollision(const CollisionData& data);
};

