#pragma once

#include "Collider.h"

namespace Ledlib {

class CircleCollider : public Collider {
public:
	float radius, radiusSquared;
	float worldRadius, worldRadiusSquared;

	CircleCollider();
	virtual void OnSetup();
	virtual void OnUpdateTransform();

	void SetRadius(float r);
};

}

