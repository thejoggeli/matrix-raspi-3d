#pragma once

#include <vector>
#include <memory>
#include "../Remote/Keys.h"

namespace Ledlib {

class ClientJoinerSlot;
class ClientJoinerListener;

class ClientJoiner {
private:
	static int initCounter;
	ClientJoiner();
public:
	static bool Init(unsigned int numSlots, bool autoReady = true);
	static void SetJoinKey(KeyCode code);
	static void SetReadyKey(KeyCode code);
	static void SetLeaveKey(KeyCode code);
	static void Reset();
	static void Update();
	static const std::vector<std::shared_ptr<ClientJoinerSlot>>& GetAllSlots();
	static const std::vector<std::shared_ptr<ClientJoinerSlot>>& GetTakenSlots();
	static std::shared_ptr<ClientJoinerSlot> FindClient(int id);
	static std::shared_ptr<ClientJoinerSlot> AddClient(int id);
	static void RemoveClient(int id);
	static bool IsEveryoneReady();
	static void AddListener(const std::shared_ptr<ClientJoinerListener>& listener);
	static bool AllSlotsTaken();
	static int GetNumTakenSlots();
};


}
