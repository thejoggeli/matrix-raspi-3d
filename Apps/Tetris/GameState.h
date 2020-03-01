#pragma once

#include "Ledlib2d/State.h"
#include <vector>

class TetrisPlayer;
class Grid;

namespace Ledlib {
	class Client;
}

class GameState : public Ledlib::State {
public:
	std::shared_ptr<Grid> grid;
	std::vector<std::unique_ptr<TetrisPlayer>> players;
	GameState();
	virtual ~GameState();
	void OnStart();
	void OnExit();
	void OnUpdate();
	void OnRender();
	void AddPlayer(Ledlib::Client* client);
};

