#pragma once

#include "Ledlib2d/State.h"

using namespace Ledlib;

namespace Ledlib {

class Bitmap;

}

class MenuState : public State {
private:
public:
	bool showHighscore = false;
	float flashTimer;
	int selection = 1;
	virtual void OnStart();
	virtual void OnEnd();
	virtual void OnUpdate();
	virtual void OnBeforeRender();
	Bitmap* GetBirdBitmap();
};
