#pragma once

#include "Ledlib2d/Entity.h"

using namespace Ledlib;

namespace Ledlib {
class Bitmap;
}

class BirdEntity : public Entity {
public:
	glm::vec2 velocity;
	glm::vec2 acceleration;
	Bitmap* bitmap;
	float flapVelocity = 50.0f;
	BirdEntity();	
	void OnStart();
	void OnUpdate();
	void OnRender();
	void OnCollision(const CollisionData &data);
};

