#pragma once

#include <string>

namespace Ledlib {

class Server;
class ServerMessage;

class ServerManager {
private:
	static int initCounter;
	ServerManager();
public:
	static bool Init();
	static void Poll();
	static int GetNumConnections();
	static int GetPort();
	static std::string GetLocalUrl();
	static std::string GetGlobalUrl();
	static void Send(std::string& string);
	static void SendMessage(ServerMessage& message, int clientId = 0);
	static void Shutdown();
};

}
