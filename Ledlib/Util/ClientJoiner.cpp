#include "ClientJoiner.h"
#include "ClientJoinerListener.h"
#include "../Remote/ClientManager.h"
#include "../Remote/Client.h"
#include "../Log.h"
#include <algorithm>

using namespace std;

namespace Ledlib {

static vector<shared_ptr<JoinedClient>> joinedClients;
static vector<shared_ptr<JoinedClient>> slots;
static vector<shared_ptr<ClientJoinerListener>> listeners;
static bool readyFlag = false;
static bool autoReady = false;
static unsigned int maxClients;

static KeyCode joinKey = KeyCode::Start;
static KeyCode leaveKey = KeyCode::B;

int ClientJoiner::initCounter = 0;

bool ClientJoiner::Init(unsigned int maxClients, bool autoReady){
	if(++initCounter > 1) return false;
	Log(LOG_INFO, "ClientJoiner", "Initializing");
	maxClients = maxClients;
	autoReady = autoReady;
	Reset();
	return true;
}

void ClientJoiner::Reset(){
	for(auto const& client: joinedClients){
		RemoveJoinedClient(client->id);
	}
	joinedClients.clear();
	slots.clear();
	for(int i = 0; i < maxClients; i++){
		slots[i] = nullptr;
	}
	readyFlag = false;
}


void ClientJoiner::SetJoinKey(KeyCode code){
	joinKey = code;
}
void ClientJoiner::SetLeaveKey(KeyCode code){
	leaveKey = code;
}

void ClientJoiner::Update(){
	for(auto const& client: ClientManager::GetAllCients()){
		if((client->OnKeyDown(joinKey))){
			shared_ptr<JoinedClient> joinedClient = FindJoinedClient(client->id);
			if(joinedClient){
				// client is ready
				if(!joinedClient->ready){
					joinedClient->ready = true;
					Log(LOG_DEBUG, "ClientJoiner", iLog << "Client (" << client->id << ") is ready");
				}
			} else if(joinedClients.size() < maxClients) {
				// client joined
				shared_ptr<JoinedClient> jc = AddJoinedClient(client->id);
				for(auto const& listener: listeners){
					listener->OnClientJoined(client->id);
				}
				Log(LOG_DEBUG, "ClientJoiner", iLog << "Client (" << client->id << ") joined in slot [" << jc->slot << "]");
			}
		} else if(client->OnKeyDown(leaveKey)){
			shared_ptr<JoinedClient> joinedClient = FindJoinedClient(client->id);
			if(joinedClient){
				if(joinedClient->ready && !autoReady){
					// client is no longer ready
					joinedClient->ready = false;
					Log(LOG_DEBUG, "ClientJoiner", iLog << "Client (" << client->id << ") is no long ready");
				} else {
					// client left
					RemoveJoinedClient(client->id);
					for(auto const& listener: listeners){
						listener->OnClientLeft(false);
					}
					Log(LOG_DEBUG, "ClientJoiner", iLog << "Client (" << client->id << ") left");
				}
			}
		}
	}
	// remove old clients
	for(auto const& joinedClient: joinedClients){
		if(!ClientManager::GetClient(joinedClient->id)){
			RemoveJoinedClient(joinedClient->id);
			for(auto const& listener: listeners){
				listener->OnClientLeft(true);
			}
			Log(LOG_DEBUG, "ClientJoiner", iLog << "Client (" << joinedClient->id << ") was disconnected");
		}
	}
	// listeners
	if(IsEveryoneReady()){
		if(!readyFlag){
			readyFlag = true;
			for(auto const& listener: listeners){
				listener->OnEveryoneReady(static_cast<int>(joinedClients.size()));
			}
			Log(LOG_DEBUG, "ClientJoiner", iLog << "Everyone is ready (" << joinedClients.size() << ")");
		}
	} else if(readyFlag) {
		readyFlag = false;
		Log(LOG_DEBUG, "ClientJoiner", iLog << "Everyone is no longer ready");
	}
}

shared_ptr<JoinedClient> ClientJoiner::FindJoinedClient(int id){
	for(shared_ptr<JoinedClient> jc: joinedClients){
		if(jc->id == id) return jc;
	}
	return nullptr;
}

shared_ptr<JoinedClient> ClientJoiner::AddJoinedClient(int id){
	shared_ptr<JoinedClient> jc = make_shared<JoinedClient>();
	jc->id = id;
	jc->ready = autoReady ? true : false;
	for(jc->slot = 0; jc->slot < maxClients; jc->slot++){
		if(slots[jc->slot] == nullptr){
			slots[jc->slot] = jc;
			break;
		};
	}
	joinedClients.push_back(jc);
	return jc;
}

void ClientJoiner::RemoveJoinedClient(int id){
	shared_ptr<JoinedClient> jc = FindJoinedClient(id);
	slots[jc->slot] = nullptr;
	joinedClients.erase(remove_if(joinedClients.begin(), joinedClients.end(),
		[&id](auto const& f){return f->id == id;}
	));
}

const vector<shared_ptr<JoinedClient>>& ClientJoiner::GetJoinedClients(){
	return joinedClients;
}

const vector<shared_ptr<JoinedClient>>& ClientJoiner::GetSlots(){
	return slots;
}

bool ClientJoiner::IsEveryoneReady(){
	if(joinedClients.size() < 1) return false;
	for(auto const& joinedClient: joinedClients){
		if(!joinedClient->ready) return false;
	}
	return true;
}

void ClientJoiner::AddListener(const shared_ptr<ClientJoinerListener>& listener){
	listeners.push_back(listener);
}

int ClientJoiner::GetNumClients(){
	return joinedClients.size();
}

bool ClientJoiner::MaxClientsReached(){
	return joinedClients.size() == maxClients;
}




}
