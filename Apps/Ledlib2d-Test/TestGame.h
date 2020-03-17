#pragma once

#include "Ledlib2d/Game.h"

using namespace Ledlib;

class TestGame : public Game {
public:
	virtual void OnSetup();
	virtual void OnStart();
};
