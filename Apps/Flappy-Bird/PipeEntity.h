#pragma once

#include "Ledlib2d/Entity.h"

using namespace Ledlib;

namespace Ledlib {
class Bitmap;
}

class PipeEntity : public Entity {
public:
	glm::vec3 originalPosition;
	float originalAngle;
	bool moving = false;
	bool rotating = false;
	Bitmap* bitmap;
	float width = 6, height = 32;
	PipeEntity();
	void OnStart();
	void OnUpdate();
	void OnRender();
};

