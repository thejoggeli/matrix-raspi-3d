#pragma once

namespace Ledlib {

class Event;

class EventListener {
public:
	EventListener();
	virtual ~EventListener();
	virtual void OnEvent(Event& event);
};

}


