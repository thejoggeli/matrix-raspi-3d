#include "ClientJoinerListener.h"

namespace Ledlib {

ClientJoinerListener::ClientJoinerListener(){}
ClientJoinerListener::~ClientJoinerListener(){}
void ClientJoinerListener::OnClientJoined(int clientId, int slotId){}
void ClientJoinerListener::OnClientLeft(int clientId, int slotId, bool disconnected){}
void ClientJoinerListener::OnEveryoneReady(int numClients){}

}
