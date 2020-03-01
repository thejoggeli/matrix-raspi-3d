#include "EventListener.h"
#include "EventManager.h"

namespace Ledlib {

EventListener::EventListener(){}
EventListener::~EventListener(){}

void EventListener::StartListening(){
	EventManager::AddEventListener(this);
}

void EventListener::StopListening(){
	EventManager::RemoveEventListener(this);
}

}
