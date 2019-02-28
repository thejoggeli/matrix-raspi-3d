#pragma once

#include <vector>
#include <memory>
#include "../Remote/Keys.h"

namespace Ledlib {

class ClientJoinerListener;

struct JoinedClient {
	int id;
	bool ready;
	int slot;
};

class ClientJoiner {
private:
	static int initCounter;
	ClientJoiner();
public:
	static bool Init(unsigned int maxClients, bool autoReady = true);
	static void SetJoinKey(KeyCode code);
	static void SetLeaveKey(KeyCode code);
	static void Reset();
	static void Update();
	static const std::vector<std::shared_ptr<JoinedClient>>& GetJoinedClients();
	static const std::vector<std::shared_ptr<JoinedClient>>& GetSlots();
	static std::shared_ptr<JoinedClient> FindJoinedClient(int id);
	static std::shared_ptr<JoinedClient> AddJoinedClient(int id);
	static void RemoveJoinedClient(int id);
	static bool IsEveryoneReady();
	static void AddListener(const std::shared_ptr<ClientJoinerListener>& listener);
	static bool MaxClientsReached();
	static int GetNumClients();
};


}
