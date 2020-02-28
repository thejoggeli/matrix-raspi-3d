#pragma once

#include "Ledlib2d/Game.h"

using namespace Ledlib;

class Game2d : public Game {
public:
	virtual void OnSetup();
	virtual void OnStart();
	virtual void OnExit();
};
