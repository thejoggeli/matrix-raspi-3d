#pragma once

#include <vector>
#include "Keys.h"
#include <memory>

namespace Ledlib {

class Client;

class ClientManager {
private:
	static int initCounter;
	ClientManager();
public:
	static bool Init();
	static void Update();
	static void AddClient(std::shared_ptr<Client> client);
	static void RemoveClient(int id);
	static std::shared_ptr<Client> GetClient(int id);
	static const std::vector<std::shared_ptr<Client>>& GetAllCients();

	static bool IsKeyDown(KeyCode code);
	static bool OnKeyDown(KeyCode code);
	static bool OnKeyUp(KeyCode code);
	static int GenerateId();

};


}

