#pragma once

#include "Ledlib2d/State.h"
#include <vector>
#include <memory>

class TetrisPlayer;
class Grid;

namespace Ledlib {
	class Client;
	class Event;
	class Bitmap;
}

class GameState : public Ledlib::State {
public:

	std::shared_ptr<Ledlib::Bitmap> bgBitmap;
	float bgAngle = 0;
	float bgHue = 0;

	std::vector<std::shared_ptr<TetrisPlayer>> players;
	GameState();
	virtual ~GameState();
	void OnStart();
	void OnExit();
	void OnUpdate();
	void OnRender();
	TetrisPlayer& AddPlayer(const std::shared_ptr<Ledlib::Client>& client);
};

