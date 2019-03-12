#pragma once

#include "Ledlib2d/State.h"

using namespace Ledlib;

class Grid;

class GameState : public State {
public:
	std::shared_ptr<Grid> grid;
	GameState();
	void OnStart();
	void OnEnd();
	void OnUpdate();
	void OnRender();
};

