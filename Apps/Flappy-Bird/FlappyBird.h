#pragma once

#include "Ledlib2d/Game.h"

using namespace Ledlib;

namespace Ledlib {
class Bitmap;
}

class FlappyBird : public Game {
public:
	Bitmap* birdBitmap = nullptr;
	int highscore = 0;
	virtual void OnSetup();
	virtual void OnStart();
	virtual void OnEnd();
};
