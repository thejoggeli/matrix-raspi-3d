#pragma once

#include "Ledlib2d/Entity.h"
#include "Ledlib/Util/ColorRgb.h"

class SnakeSegment;

using namespace Ledlib;

class Snake : public Entity {
public:
	ColorRgb color = ColorRgb::RED;
	std::weak_ptr<SnakeSegment> head;
	void OnCreate() override;
	void OnStart() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnAfterRender();
	void OnExit();
	std::shared_ptr<SnakeSegment> LastSegment();
	void Grow();
	void OnCollision(const CollisionData& data) override;
};

