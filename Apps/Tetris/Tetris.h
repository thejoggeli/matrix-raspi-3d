#pragma once

#include "Ledlib2d/Game.h"

using namespace Ledlib;

class Tetris : public Game {
public:
	virtual void OnSetup();
	virtual void OnStart();
};
