#pragma once

#include "ShaderState.h"

using namespace Ledlib;

class Basic : public ShaderState {
public:
	Basic();
	void OnStart();
	void OnUpdate();
	void OnRender();
};

