#include "ClientJoinerSlot.h"
#include "ClientJoiner.h"
#include "Ledlib/Remote/ClientManager.h"

namespace Ledlib {

ClientJoinerSlot::ClientJoinerSlot(unsigned int slotId){
	_slotId = slotId;
}

bool ClientJoinerSlot::IsTaken(){
	return clientId > 0;
}

void ClientJoinerSlot::SetReady(bool ready){
	_ready = ready;
}

void ClientJoinerSlot::SetClientId(int id){
	_clientId = id;
}
bool ClientJoinerSlot::IsReady(){
	return _ready;
}
std::shared_ptr<Client> ClientJoinerSlot::GetClient(){
	return ClientManager::GetClient(_clientId);
}

}

