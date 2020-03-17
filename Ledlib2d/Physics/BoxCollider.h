#pragma once

#include "Collider.h"

namespace Ledlib {

class BoxCollider : public Collider {
public:
	float width, height;
	float worldWidth, worldHeight;
	float worldLeft, worldRight, worldTop, worldBottom;
	BoxCollider();
	virtual void OnSetup();
	virtual void OnUpdateTransform();
	virtual void OnDebugDraw();

	void SetSize(float w, float h);


};

}


