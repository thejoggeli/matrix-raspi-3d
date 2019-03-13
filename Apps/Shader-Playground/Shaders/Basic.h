#pragma once

#include "ShaderState.h"
#include <glm/vec4.hpp>

using namespace Ledlib;

class Basic : public ShaderState {
public:
	glm::vec4 args;
	Basic();
	void OnStart();
	void OnUpdate();
	void OnRender();
};

