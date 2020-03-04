#include "SeasocksServer.h"
#include "ServerMessage.h"
#include "ClientManager.h"
#include "Client.h"
#include "../Events/EventManager.h"
#include "../Events/Event.h"
#include "../Config.h"
#include "../Log.h"
#include <algorithm>

using namespace seasocks;
using namespace std;

namespace Ledlib {

SeasocksServer::SeasocksServer(){}

bool SeasocksServer::Init(){
	Logger::Level loggerLevel = static_cast<Logger::Level>(Config::GetInt("insrv_logger_level"));
	port = Config::GetInt("insrv_port");
	websocketPath = "/ws";
	localUrl = "ws://localhost:" + to_string(port) + websocketPath;
	globalUrl = "undefined";

	// logger
	logger = std::make_shared<PrintfLogger>(loggerLevel);

	// server
	std::string staticPath = LEDLIB_WEB_PATH;
	server = make_shared<seasocks::Server>(logger);
//	server->setClientBufferSize(20*1024); // 20kB
	server->setStaticPath(staticPath.c_str());
	handler = make_shared<SeasocksHandler>(server);
	server->addWebSocketHandler(websocketPath.c_str(), handler);
	server->startListening(port);

	// output
	Log(LOG_DEBUG, "SeasocksServer", iLog << "StaticPath: " << LEDLIB_WEB_PATH);
	Log(LOG_DEBUG, "SeasocksServer", iLog << "LoggerLevel set to " << Logger::levelToString(loggerLevel));
	Log(LOG_INFO, "SeasocksServer", iLog << "Local access: " << GetLocalUrl());
	Log(LOG_INFO, "SeasocksServer", iLog << "Global access: " << GetGlobalUrl());

	return true;
}

void SeasocksServer::Send(std::string& string){
	for(auto const& it: handler->connections){
		it->send(string);
	}
}
void SeasocksServer::SendMessage(ServerMessage& message, int clientId){
	string& str = message.GetJsonString();
	Log(LOG_DEBUG, "SeasocksServer", iLog << "Send (id=" << clientId << ") " << str);
	if(clientId == 0){
		for(auto const& it: handler->connections){
			it->send(str);
		}
	} else {
		for(auto const& it: handler->connections){
			shared_ptr<Client> c = handler->GetClientByConnection(it);
			if(c && c->id == clientId){
				it->send(str);
			}
		}
	}
}

void SeasocksServer::Poll(){
	server->poll(0);
}

int SeasocksServer::GetNumConnections(){
	return static_cast<int>(handler->connections.size());
}

int SeasocksServer::GetPort(){
	return port;
}

string SeasocksServer::GetLocalUrl(){
	return localUrl;
}

string SeasocksServer::GetGlobalUrl(){
	return globalUrl;
}
void SeasocksServer::Shutdown(){
	for(auto const& it: handler->connections){
		it->close();
	}
	server->terminate();
	server = nullptr;
}

SeasocksHandler::SeasocksHandler(shared_ptr<seasocks::Server> server){
	this->server = server;
	logInput = Config::GetBool("log_info_server_input", true);
	logOutput = Config::GetBool("log_info_server_output", true);
}

void SeasocksHandler::onConnect(WebSocket* connection) {
	int clientId = ClientManager::GenerateId();
	// alloc memory for client
	shared_ptr<Client> client = make_shared<Client>(clientId);
	// add client to client manager
	ClientManager::AddClient(client);
	// broadcast client connected event
	shared_ptr<MessageEvent> event = make_shared<MessageEvent>("client_connected");
	event->clientId = client->id;
	event->fromClient = false;
	EventManager::AddEvent(event);
	// add connection to vector
	connections.push_back(connection);
	// add connection to map
	clientMap[connection] = client;
};

void SeasocksHandler::onData(WebSocket* connection, const char* data) {
	if(logInput){
		Log(LOG_INFO, "SeasocksServer", iLog << "OnData: " << data);
	}
	shared_ptr<Client> client = GetClientByConnection(connection);
	client->AddMessage(data);
};

void SeasocksHandler::onDisconnect(WebSocket* connection) {
	shared_ptr<Client> client = GetClientByConnection(connection);
	if(client == nullptr){
		Log(LOG_ERROR, "SeasocksServer", "Client not found in client map");
		return;
	}
	// remove client from client manager
	ClientManager::RemoveClient(client->id);
	// broadcast client connected event
	shared_ptr<MessageEvent> event = make_shared<MessageEvent>("client_disconnected");
	event->clientId = client->id;
	event->fromClient = false;
	EventManager::AddEvent(event);
	// remove connection from vector
	auto it = std::find(connections.begin(), connections.end(), connection);
	connections.erase(it);
	// remove connection from map
	clientMap.erase(connection);
};

shared_ptr<Client> SeasocksHandler::GetClientByConnection(WebSocket* connection){
	auto search = clientMap.find(connection);
	if(search == clientMap.end()){
		return nullptr;
	}
	return search->second;
}

}

