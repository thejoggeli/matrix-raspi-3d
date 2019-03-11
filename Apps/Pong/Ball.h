#pragma once

#include "Ledlib2d/Entity.h"

using namespace Ledlib;

class PipeEntity : public Entity {
public:
	PipeEntity();
	void OnStart();
	void OnUpdate();
	void OnRender();
};

