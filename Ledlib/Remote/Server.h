#pragma once

#include <string>

namespace Ledlib {

class ServerMessage;

class Server {
private:
	int initCounter = 0;
public:
	Server();
	virtual ~Server();
	virtual bool Init();
	virtual void Poll() = 0;
	virtual int GetNumConnections() = 0;
	virtual int GetPort() = 0;
	virtual std::string GetLocalUrl() = 0;
	virtual std::string GetGlobalUrl() = 0;
	virtual void Send(std::string& string) = 0;
	virtual void SendMessage(ServerMessage& message, int clientId = 0) = 0;
	virtual void Shutdown() = 0;
};

}
