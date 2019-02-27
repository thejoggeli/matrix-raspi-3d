#include "EventManager.h"
#include "Event.h"
#include "EventListener.h"
#include "../Log.h"
#include "../Config.h"
#include <algorithm>
#include <unordered_map>

namespace Ledlib {

static bool logEvents = false;

int EventManager::initCounter = 0;

static std::vector<EventListener*> listeners;
static std::vector<Event*> events;
static std::vector<Event*> newEvents;
static std::unordered_map<std::string, EventType> KeyCode;

bool EventManager::Init(){
	if(++initCounter > 1) return false;
	KeyCode.insert({"msg", EventType::Message});
	KeyCode.insert({"inp", EventType::Input});
	KeyCode.insert({"joy", EventType::Joystick});
	logEvents = Config::GetBool("log_info_events", true);
	return true;
}

void EventManager::AddEvent(Event* event){
	if(logEvents){
		Log(LOG_INFO, "EventManager", event->ToString());
	}
	newEvents.push_back(event);
}

void EventManager::NextGeneration(){
	for(auto const &event: events){
		delete event;
	}
	events.clear();
	events.insert(events.end(), newEvents.begin(), newEvents.end());
	newEvents.clear();
}

void EventManager::Update(){
	for(auto& event: events){
		for(auto const& listener: listeners){
			listener->OnEvent(*event);
		}
	}
}

std::vector<Event*>& EventManager::GetAllEvents(){
	return events;
}

MessageEvent* EventManager::GetMessageEvent(const std::string& str){
	for(auto const &event: events){
		if(event->type == EventType::Message){
			MessageEvent* e = static_cast<MessageEvent*>(event);
			if(e->message == str) return e;
		}
	}
	return nullptr;
}

Event* EventManager::ParseMessage(std::string str){
	Event* event = nullptr;
	// check if string length is at least 3
	int strlen = str.length();
	if(strlen < 3){
		Log(LOG_ERROR, "EventManager", "Invalid message: " + str);
		return nullptr;
	}
	// get event type
	std::string sType = str.substr(0,3);
	auto search = KeyCode.find(sType);
	if(search == KeyCode.end()){
		Log(LOG_ERROR, "EventManager", "Invalid event type: " + sType);
		return nullptr;
	}
	EventType eventType = search->second;
	// create event
	switch(eventType){
	case EventType::Message: {
		// get command
		unsigned long pos = str.find(":");
		std::string message = str.substr(3, pos-3);
		MessageEvent* messageEvent = new MessageEvent(message);
		// get parameters
		std::string remaining = str.substr(pos+1);
		pos = remaining.find("/");
		while(pos < remaining.size()){
			unsigned long length = stoul(remaining.substr(0, pos));
			std::string param = remaining.substr(pos+1, length);
			remaining = remaining.substr(pos+1+length);
			pos = remaining.find("/");
			messageEvent->AddParam(param);
		}
		event = static_cast<Event*>(messageEvent);
		break;
	}
	case EventType::Input: {
		// input event
		// [0-2] = "inp"
		// [3] 0=pressed, 1=released
		// [4+] keycode
		if(strlen < 5){
			Log(LOG_ERROR, "EventManager", "Input event too short");
			return nullptr;
		}
		int state = std::stoi(str.substr(3,1));
		int code = std::stoi(str.substr(4));
		if(state < 0 || state > 1){
			Log(LOG_ERROR, "EventManager", "Invalid input key state value");
			return nullptr;
		}
		if(state < 0 || code > static_cast<int>(KeyCode::KEY_CODE_MAX)){
			Log(LOG_ERROR, "EventManager", "Invalid input key code value");
			return nullptr;
		}
		event = new InputEvent(static_cast<enum KeyCode>(code), static_cast<KeyState>(state));
		break;
	}
	case EventType::Joystick: {
		// input event
		// [0-2] = "joy"
		// [3] KeyState 0=Pressed, 1=Released, 2=JoystickMove
		// [4] KeyCode 0=LeftJoystick, 1=RightJoystick
		// [5] Delimiter
		// [6-14] x position
		// [15] Delimiter
		// [16-24] y position
		int state = std::stoi(str.substr(3,1));
		int code = std::stoi(str.substr(4,1));
		enum KeyCode keyCode = (code == 0) ? KeyCode::LeftJoystick : KeyCode::RightJoystick;
		JoystickEvent* jev = new JoystickEvent(keyCode, static_cast<KeyState>(state));
		if(state == 2){
			float x = std::stof(str.substr(6,9));
			float y = std::stof(str.substr(16,9));
			jev->position = Vector2f(x, y);
		} else {
			jev->position = Vector2f(0, 0);
		}
		event = jev;
	}
	}
	if(event == nullptr){
		Log(LOG_ERROR, "EventManager", "Parser couldn't handle event type: " + sType);
	}
	return event;
}

void EventManager::AddEventListener(EventListener *listener){
	listeners.push_back(listener);
}
void EventManager::RemoveEventListener(EventListener *listener){
	listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
}


}

