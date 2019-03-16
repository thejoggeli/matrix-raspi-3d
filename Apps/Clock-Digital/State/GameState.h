#pragma once

#include "Ledlib2d/State.h"

using namespace Ledlib;

class Clock;

namespace Ledlib {
class ShaderBox;
}

class GameState : public State {
private:
public:
	std::shared_ptr<ShaderBox> border;
	std::shared_ptr<Clock> clock;
	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnAfterRender();
	virtual void OnEnd();
};
