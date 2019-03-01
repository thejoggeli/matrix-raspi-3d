#pragma once

namespace Ledlib {

struct ClientJoinerListener {
	ClientJoinerListener();
	virtual ~ClientJoinerListener();
	virtual void OnEveryoneReady(int numClients);
	virtual void OnClientJoined(int clientId, int slotId);
	virtual void OnClientLeft(int clientId, int slotId, bool disconnected);
};

}
