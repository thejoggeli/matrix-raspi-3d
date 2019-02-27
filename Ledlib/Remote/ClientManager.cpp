#include "ClientManager.h"
#include "Client.h"
#include "../Log.h"
#include <algorithm>
#include <unordered_map>
#include <vector>

namespace Ledlib {

int ClientManager::initCounter = 0;

static int currentClientId = 1;
static std::vector<Client*> clientsVector;
static std::unordered_map<int, Client*> clientsMap;

bool ClientManager::Init(){
	if(++initCounter > 1) return false;
	return true;
}

void ClientManager::Update(){
	for(auto const &client: GetAllCients()){
		client->Update();
	}
}

void ClientManager::AddClient(Client *client){
	clientsVector.push_back(client);
	clientsMap.insert({client->id, client});
/*	if(iLog.infos.clients){
		Log(LOG_INFO, "ClientManager", iLog << "Client (" << client->id << ") added");
	} */
}

void ClientManager::RemoveClient(int id){
	Client *client = GetClient(id);
	if(client == nullptr){
		Log(LOG_ERROR, "ClientManager", iLog << "Can't remove client (id=" << id << "): not found in map");
		return;
	}
	std::vector<Client*>::iterator it = std::find_if(clientsVector.begin(), clientsVector.end(), [&id](Client* c){
		return c->id == id;
	});
	if(it == clientsVector.end()){
		Log(LOG_ERROR, "ClientManager", "Client found in map but not found in vector");
		return;
	}
	clientsVector.erase(it);
	clientsMap.erase(id);
/*	if(iLog.infos.clients){
		Log(LOG_INFO, "ClientManager", iLog << "Client removed: " << client->id);
	} */
}

Client* ClientManager::GetClient(int id){
	auto search = clientsMap.find(id);
	if(search != clientsMap.end()){
		return search->second;
	}
	return nullptr;
};

const std::vector<Client*>& ClientManager::GetAllCients() {
	return clientsVector;
}

bool ClientManager::IsKeyDown(KeyCode code){
	for(std::vector<Client*>::iterator it = clientsVector.begin(); it != clientsVector.end(); ++it){
		if((*it)->IsKeyDown(code)) return true;
	}
	return false;
};
bool ClientManager::OnKeyDown(KeyCode code){
	for(std::vector<Client*>::iterator it = clientsVector.begin(); it != clientsVector.end(); ++it){
		if((*it)->OnKeyDown(code)) return true;
	}
	return false;
};
bool ClientManager::OnKeyUp(KeyCode code){
	for(std::vector<Client*>::iterator it = clientsVector.begin(); it != clientsVector.end(); ++it){
		if((*it)->OnKeyUp(code)) return true;
	}
	return false;
};

int ClientManager::GenerateId(){
	return currentClientId++;
}

}
