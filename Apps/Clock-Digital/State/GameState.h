#pragma once

#include "Ledlib2d/State.h"

using namespace Ledlib;

class Clock;

class GameState : public State {
private:
public:
	std::shared_ptr<Clock> clock;
	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnEnd();
};
