#pragma once

#include "Ledlib2d/Entity.h"
#include "Ledlib/Util/ColorRgb.h"

using namespace Ledlib;

class Asteroid : public Entity {
public:
	float hue;
	ColorRgb color;
	std::vector<glm::vec2> vertices;
	glm::vec3 velocity;
	float angularVelocity;
	Asteroid();
	void Generate(float minRadius, float maxRadius, int minPoints, int maxPoints);
	void OnStart();
	void OnUpdate();
	void OnRender();
	void OnEnd();
	virtual void OnCollision(const CollisionData& data);
};
