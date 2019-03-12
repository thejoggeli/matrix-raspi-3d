#pragma once

#include "Ledlib/Util/Timer.h"
#include "Ledlib2d/State.h"

namespace Ledlib {
class Bitmap;
}

using namespace Ledlib;

class MenuState : public State {
private:
	std::unique_ptr<Bitmap> bgBitmap;
	Timer startTimer;
	bool startTimerRunning = false;
public:
	MenuState();
	~MenuState();
	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnRender();
};
