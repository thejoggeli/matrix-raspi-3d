#pragma once

#include <string>
#include <vector>

namespace Ledlib {

class Event;
class MessageEvent;
class EventListener;

class EventManager {
private:
	static int initCounter;
	EventManager();
public:
	static bool Init();
	static void AddEvent(Event* event);
	static void NextGeneration();
	static Event* ParseMessage(std::string str);
	static MessageEvent* GetMessageEvent(const std::string& str);
	static std::vector<Event*>& GetAllEvents();
	static void AddEventListener(EventListener* listener);
	static void RemoveEventListener(EventListener* listener);
	static void Update();
};


}

