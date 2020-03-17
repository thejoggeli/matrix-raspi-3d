#pragma once

#include "Ledlib2d/Entity.h"
#include "Ledlib/Math/Rect4f.h"
#include "Ledlib/Util/Timer.h"

using namespace Ledlib;

class Ball : public Entity {
public:

	Timer oldPositionTimer;
	glm::vec2 oldPosition;
	std::vector<glm::vec2> oldPositions;

	Rect4f bounds;
	float radius = 0.5f;
	glm::vec2 velocity;
	float velocityGain = 0.5f;

	Ball();
	void OnStart();
	void OnUpdate();
	void OnRender();
	void OnCollision(const CollisionData& data);

};

