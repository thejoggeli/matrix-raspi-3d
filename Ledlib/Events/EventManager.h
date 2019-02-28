#pragma once

#include <string>
#include <vector>
#include <memory>

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
	static void AddEvent(const std::shared_ptr<Event>& event);
	static void NextGeneration();
	static std::shared_ptr<Event> ParseMessage(const std::string& str);
	static std::shared_ptr<MessageEvent> GetMessageEvent(const std::string& str);
	static std::vector<std::shared_ptr<Event>>& GetAllEvents();
	static void AddEventListener(const std::shared_ptr<EventListener>& listener);
	static void RemoveEventListener(const std::shared_ptr<EventListener>& listener);
	static void Update();
};


}

