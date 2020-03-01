#pragma once

#include <memory>

namespace Ledlib {
class Client;
}

class TetrisPlayer {
public:
	std::weak_ptr<Ledlib::Client> client;
	std::shared_ptr<Grid> grid;
	TetrisPlayer();
};

