#pragma once

#include "Ledlib2d/Game.h"

using namespace Ledlib;

class ShaderPlayground : public Game {
public:
	std::string file;
	ShaderPlayground();
	void OnSetup();
	void OnStart();
};

