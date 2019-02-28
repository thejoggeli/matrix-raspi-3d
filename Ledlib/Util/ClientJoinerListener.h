#pragma once

namespace Ledlib {

struct ClientJoinerListener {
	ClientJoinerListener();
	virtual ~ClientJoinerListener();
	virtual void OnEveryoneReady(int numClients);
	virtual void OnClientJoined(int id);
	virtual void OnClientLeft(bool disconnected);
};

}
