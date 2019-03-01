#pragma once

#include "Events/EventListener.h"

namespace Ledlib {

class Event;

class LedlibEventHandler : public EventListener {
public:
	void OnEvent(Event& event);
};

}
