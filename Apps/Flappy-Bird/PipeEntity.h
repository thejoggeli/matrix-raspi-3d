#pragma once

#include "Ledlib2d/Entity.h"

using namespace Ledlib;

namespace Ledlib {
class Bitmap;
class Entity;
}

class PipeEntity : public Entity {
public:
	Entity* colliderEntity;
	Bitmap* bitmap;
	float width = 6, height = 32;
	PipeEntity();
	void OnStart();
	void OnUpdate();
	void OnRender();
};

