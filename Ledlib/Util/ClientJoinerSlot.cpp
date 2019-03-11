#include "ClientJoinerSlot.h"
#include "ClientJoiner.h"

namespace Ledlib {

ClientJoinerSlot::ClientJoinerSlot(unsigned int slotId) : slotId(slotId){}

bool ClientJoinerSlot::IsTaken(){
	return clientId > 0;
}

void ClientJoinerSlot::SetReady(bool ready){
	_ready = ready;
}

void ClientJoinerSlot::SetClientId(int id){
	_clientId = id;
}

}

