#pragma once

#include <vector>
#include "Keys.h"

namespace Ledlib {

class Client;

class ClientManager {
private:
	static int initCounter;
	ClientManager();
public:
	static bool Init();
	static void Update();
	static void AddClient(Client *client);
	static void RemoveClient(int id);
	static Client* GetClient(int id);
	static const std::vector<Client*> & GetAllCients();

	static bool IsKeyDown(KeyCode code);
	static bool OnKeyDown(KeyCode code);
	static bool OnKeyUp(KeyCode code);
	static int GenerateId();

};


}

