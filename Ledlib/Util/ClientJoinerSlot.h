#pragma once

namespace Ledlib {

class ClientJoinerSlot {
private:
	bool _ready = false;
	int _clientId = 0;
public:
	const unsigned int slotId;
	const bool& ready = _ready;
	const int& clientId = _clientId;
	ClientJoinerSlot(unsigned int slotId);
	bool IsTaken();
	void SetReady(bool ready);
	void SetClientId(int id);
};

}
