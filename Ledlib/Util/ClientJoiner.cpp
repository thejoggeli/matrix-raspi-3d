#include "ClientJoiner.h"
#include "ClientJoinerSlot.h"
#include "ClientJoinerListener.h"
#include "../Remote/ClientManager.h"
#include "../Remote/Client.h"
#include "../Log.h"
#include <algorithm>

using namespace std;

namespace Ledlib {

static vector<shared_ptr<ClientJoinerSlot>> takenSlots;
static vector<shared_ptr<ClientJoinerSlot>> slots;
static vector<shared_ptr<ClientJoinerListener>> listeners;
static bool readyFlag = false;
static bool autoReady = false;
static unsigned int numSlots;

static KeyCode joinKey = KeyCode::Start;
static KeyCode readyKey = KeyCode::Start;
static KeyCode leaveKey = KeyCode::B;

int ClientJoiner::initCounter = 0;

bool ClientJoiner::Init(unsigned int numSlots_, bool autoReady_){
	initCounter++;
	Log(LOG_INFO, "ClientJoiner", "Initializing");
	numSlots = numSlots_;
	autoReady = autoReady_;
	Reset();
	return true;
}

void ClientJoiner::Reset(){
	if(initCounter < 1) throw "ClientJoiner is not initialized";
	takenSlots.clear();
	slots.clear();
	slots.reserve(numSlots);
	for(int i = 0; i < numSlots; i++){
		slots.push_back(make_shared<ClientJoinerSlot>(i));
	}
	readyFlag = false;
}


void ClientJoiner::SetJoinKey(KeyCode code){
	joinKey = code;
}
void ClientJoiner::SetReadyKey(KeyCode code){
	readyKey = code;
}
void ClientJoiner::SetLeaveKey(KeyCode code){
	leaveKey = code;
}

void ClientJoiner::Update(){
	if(initCounter < 1) throw "ClientJoiner is not initialized";
	for(auto const& client: ClientManager::GetAllCients()){
		bool cancel = false;
		if((client->OnKeyDown(joinKey))){
			shared_ptr<ClientJoinerSlot> slot = FindClient(client->id);
			if(slot){
				// nothing
			} else if(takenSlots.size() < numSlots) {
				// joined
				slot = AddClient(client->id);
				for(auto const& listener: listeners){
					listener->OnClientJoined(client->id, slot->slotId);
				}
				Log(LOG_DEBUG, "ClientJoiner", iLog << "Client (id=" << client->id << ") joined in slot [" << slot->slotId << "]");
				cancel = true;
			}
		}
		if(client->OnKeyDown(readyKey) && !cancel){
			shared_ptr<ClientJoinerSlot> slot = FindClient(client->id);
			if(slot && !slot->IsReady()){
				// ready
				slot->SetReady(true);
				Log(LOG_DEBUG, "ClientJoiner", iLog << "Client (id=" << client->id << ") is ready");
				cancel = true;
			}
		}
		if(client->OnKeyDown(leaveKey) && !cancel){
			shared_ptr<ClientJoinerSlot> slot = FindClient(client->id);
			if(slot){
				if(slot->IsReady() && !autoReady){
					// client is no longer ready
					slot->SetReady(false);
					Log(LOG_DEBUG, "ClientJoiner", iLog << "Client (id=" << client->id << ") is no long ready");
				} else {
					// client left
					RemoveClient(client->id);
					for(auto const& listener: listeners){
						listener->OnClientLeft(client->id, slot->slotId, false);
					}
					Log(LOG_DEBUG, "ClientJoiner", iLog << "Client (id=" << client->id << ") left in slot [" << slot->slotId << "]");
				}
			}
		}
	}
	// remove old clients
	for(auto const& slot: takenSlots){
		if(!ClientManager::GetClient(slot->clientId)){
			int clientId = slot->clientId;
			RemoveClient(slot->clientId);
			for(auto const& listener: listeners){
				listener->OnClientLeft(clientId, slot->slotId, true);
			}
			Log(LOG_DEBUG, "ClientJoiner", iLog << "Client (" << clientId << ") was disconnected in slot [" << slot->slotId << "]");
		}
	}
	// listeners
	if(IsEveryoneReady()){
		if(!readyFlag){
			readyFlag = true;
			for(auto const& listener: listeners){
				listener->OnEveryoneReady(static_cast<int>(takenSlots.size()));
			}
			Log(LOG_DEBUG, "ClientJoiner", iLog << "Everyone is ready (n=" << takenSlots.size() << ")");
		}
	} else if(readyFlag) {
		readyFlag = false;
		Log(LOG_DEBUG, "ClientJoiner", iLog << "No longer ready");
	}
}

shared_ptr<ClientJoinerSlot> ClientJoiner::FindClient(int id){
	if(++initCounter < 1) throw "ClientJoiner is not initialized";
	for(shared_ptr<ClientJoinerSlot> slot: takenSlots){
		if(slot->clientId == id) return slot;
	}
	return nullptr;
}

shared_ptr<ClientJoinerSlot> ClientJoiner::AddClient(int id){
	if(++initCounter < 1) throw "ClientJoiner is not initialized";
	unsigned int slotId;
	for(slotId = 0; slotId < numSlots; slotId++){
		if(!slots[slotId]->IsTaken()) break;
	}
	if(slotId < numSlots){
		shared_ptr<ClientJoinerSlot> slot = slots[slotId];
		slot->SetClientId(id);
		slot->SetReady(autoReady ? true : false);
		takenSlots.push_back(slot);
		return slot;
	}
	return nullptr;
}
void ClientJoiner::RemoveClient(int id){
	shared_ptr<ClientJoinerSlot> slot = FindClient(id);
	if(slot){
		slot->SetClientId(0);
		slot->SetReady(false);
		takenSlots.erase(remove_if(takenSlots.begin(), takenSlots.end(),
			[&slot](auto const& f){return f->slotId == slot->slotId;}
		));
	}
}

const vector<shared_ptr<ClientJoinerSlot>>& ClientJoiner::GetAllSlots(){
	if(++initCounter < 1) throw "ClientJoiner is not initialized";
	return slots;
}

const vector<shared_ptr<ClientJoinerSlot>>& ClientJoiner::GetTakenSlots(){
	if(++initCounter < 1) throw "ClientJoiner is not initialized";
	return takenSlots;
}

bool ClientJoiner::IsEveryoneReady(){
	if(++initCounter < 1) throw "ClientJoiner is not initialized";
	if(takenSlots.size() < 1) return false;
	for(auto const& slot: takenSlots){
		if(!slot->IsReady()) return false;
	}
	return true;
}

void ClientJoiner::AddListener(const shared_ptr<ClientJoinerListener>& listener){
	if(++initCounter < 1) throw "ClientJoiner is not initialized";
	listeners.push_back(listener);
}

int ClientJoiner::GetNumTakenSlots(){
	if(++initCounter < 1) throw "ClientJoiner is not initialized";
	return takenSlots.size();
}

bool ClientJoiner::AllSlotsTaken(){
	if(++initCounter < 1) throw "ClientJoiner is not initialized";
	return takenSlots.size() == numSlots;
}

}
