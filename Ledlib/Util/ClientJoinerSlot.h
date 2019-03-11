#pragma once

#include <memory>

namespace Ledlib {

class Client;

class ClientJoinerSlot {
private:
	bool _ready = false;
	unsigned int _slotId;
	int _clientId = 0;
public:
	const unsigned int& slotId = _slotId;
	const int& clientId = _clientId;
	ClientJoinerSlot(unsigned int slotId);
	std::shared_ptr<Client> GetClient();
	bool IsTaken();
	void SetReady(bool ready);
	bool IsReady();
	void SetClientId(int id);
	void Open();
};

}
