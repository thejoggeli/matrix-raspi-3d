#pragma once

#include <vector>
#include "Ledlib2d/State.h"
#include "Ledlib/Util/Timer.h"
#include "Ledlib/Util/ColorHsl.h"
#include "Ledlib2d/WeakPointerList.h"

using namespace Ledlib;

namespace Ledlib {
class Client;
class Bitmap;
}

class Ball;
class Paddle;

class GameState : public State {
public:	

	bool playing = false;

	std::unique_ptr<Bitmap> bgBitmap;
	Timer beatTimer;
	Timer introTimer;
	ColorHsl borderColor;
	std::vector<std::weak_ptr<Client>> startClients;
	float bgAngle = 0;
	float bgHue = 0;

	WeakPointerList<Paddle> paddles;
	WeakPointerList<Ball> balls;

	GameState();
	~GameState();
	void OnStart();
	void OnUpdate();
	void OnBeforeRender();
	void OnAfterRender();

	void AssignClientToFreePaddle(std::shared_ptr<Client> client);
	void AddStartClient(std::shared_ptr<Client> client);
	std::shared_ptr<Ball> GetBall();
};

