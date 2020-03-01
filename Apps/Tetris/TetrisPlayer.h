#pragma once

#include <memory>
#include "Grid.h"

namespace Ledlib {
class Client;
}

class TetrisPlayer {
public:
	std::weak_ptr<Ledlib::Client> client;
	Grid grid;
	TetrisPlayer();
	~TetrisPlayer();
};

