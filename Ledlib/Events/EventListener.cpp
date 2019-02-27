#include "EventListener.h"
#include "EventManager.h"

namespace Ledlib {

EventListener::EventListener(){
	EventManager::AddEventListener(this);
}
EventListener::~EventListener(){
	EventManager::RemoveEventListener(this);
}
void EventListener::OnEvent(Event& event){}

}
