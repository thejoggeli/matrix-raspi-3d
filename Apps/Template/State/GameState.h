#pragma once

#include "Ledlib2d/State.h"

using namespace Ledlib;

class GameState : public State {
private:
public:
	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnEnd();
};
