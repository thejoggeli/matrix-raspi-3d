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

	std::shared_ptr<Bitmap> bgBitmap;
	float bgAngle = 0;
	float bgHue = 0;
	Timer beatTimer;
	Timer introTimer;
	ColorHsl borderColor;
	std::vector<std::weak_ptr<Client>> startClients;

	WeakPointerList<Paddle> paddles;
	WeakPointerList<Ball> balls;

	GameState();
	~GameState();
	void OnStart();
	void OnUpdate();
	void OnRender();
	void OnAfterRender();

	void AssignClientToFreePaddle(std::shared_ptr<Client> client);
	void AddStartClient(std::shared_ptr<Client> client);
	std::shared_ptr<Ball> GetBall();
};

