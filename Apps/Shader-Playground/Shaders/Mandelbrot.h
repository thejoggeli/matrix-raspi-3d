#pragma once

#include "ShaderState.h"

using namespace Ledlib;

class Mandelbrot : public ShaderState {
public:
	Mandelbrot();
	void OnStart();
	void OnUpdate();
	void OnRender();
};

