#pragma once
#include "Server.h"
#include <seasocks/Server.h>
#include <seasocks/WebSocket.h>
#include <seasocks/StringUtil.h>
#include <seasocks/PrintfLogger.h>
#include <unordered_map>
#include <vector>
#include <memory>

namespace Ledlib {

class Client;
class SeasocksHandler;

class SeasocksServer : public Ledlib::Server {
private:
    int port;
	std::string websocketPath;
	std::string localUrl;
	std::string globalUrl;
public:
	std::shared_ptr<seasocks::Server> server;
	std::shared_ptr<seasocks::PrintfLogger> logger;
	std::shared_ptr<SeasocksHandler> handler;
    SeasocksServer();
    bool Init();
    void Poll();
    int GetNumConnections();
    int GetPort();
	std::string GetLocalUrl();
	std::string GetGlobalUrl();
	virtual void Send(std::string& string);
	virtual void SendMessage(ServerMessage& message, int clientId);
	virtual void Shutdown();
};

class SeasocksHandler : public seasocks::WebSocket::Handler {
private:
	bool logInput = false;
	bool logOutput = false;
public:
	std::shared_ptr<seasocks::Server> server;
	std::vector<seasocks::WebSocket*> connections;
	std::unordered_map<seasocks::WebSocket*, std::shared_ptr<Client>> clientMap;
	explicit SeasocksHandler(std::shared_ptr<seasocks::Server> server);
	virtual void onConnect(seasocks::WebSocket* connection) override;
	virtual void onData(seasocks::WebSocket* connection, const char* data) override;
	virtual void onDisconnect(seasocks::WebSocket* connection) override;
	std::shared_ptr<Client> GetClientByConnection(seasocks::WebSocket* connection);
};

}
