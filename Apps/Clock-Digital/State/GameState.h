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

	float ex1;
	float ey1;
	float ez1;
	float ex11;
	float ey11;
	float ez11;
	float ex2;
	float ey2;
	float ez2;
	float ex22;
	float ey22;
	float ez22;

	std::shared_ptr<ShaderBox> border;
	std::shared_ptr<Clock> clock;
	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnAfterRender();
	virtual void OnExit();
};
