#pragma once

#include <memory>

namespace Ledlib {

class Event;

class EventListener : public std::enable_shared_from_this<EventListener> {
public:
	EventListener();
	virtual ~EventListener();
	void StartListening();
	void StopListening();
	virtual void OnEvent(Event& event) = 0;
};

}

