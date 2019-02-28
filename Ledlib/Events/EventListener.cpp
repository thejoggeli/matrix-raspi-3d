#include "EventListener.h"
#include "EventManager.h"

namespace Ledlib {

EventListener::EventListener(){}
EventListener::~EventListener(){}

void EventListener::StartListening(){
	EventManager::AddEventListener(shared_from_this());
}

void EventListener::StopListening(){
	EventManager::RemoveEventListener(shared_from_this());
}

}
