#include "ClientJoinerListener.h"

namespace Ledlib {

ClientJoinerListener::ClientJoinerListener(){}
ClientJoinerListener::~ClientJoinerListener(){}
void ClientJoinerListener::OnClientJoined(int id){}
void ClientJoinerListener::OnClientLeft(bool disconnected){}
void ClientJoinerListener::OnEveryoneReady(int numClients){}

}
