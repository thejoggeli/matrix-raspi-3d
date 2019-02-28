#include "ServerManager.h"
#include "SeasocksServer.h"
#include "../Log.h"

namespace Ledlib {

static Server* instance;

int ServerManager::initCounter = 0;

ServerManager::ServerManager(){}

bool ServerManager::Init(){
	if(++initCounter > 1) return false;
	Log(LOG_INFO, "ServerManager", "Initializing");
	instance = new SeasocksServer();
	return instance->Init();
}
void ServerManager::Poll(){
	instance->Poll();
}
int ServerManager::GetNumConnections(){
	return instance->GetNumConnections();
}
int ServerManager::GetPort(){
	return instance->GetPort();
}
std::string ServerManager::GetLocalUrl(){
	return instance->GetLocalUrl();
}
std::string ServerManager::GetGlobalUrl(){
	return instance->GetGlobalUrl();
}
void ServerManager::Send(std::string& string){
	instance->Send(string);
}
void ServerManager::SendMessage(ServerMessage& message, int clientId){
	instance->SendMessage(message, clientId);
}
void ServerManager::Shutdown(){
	instance->Shutdown();
}
}
