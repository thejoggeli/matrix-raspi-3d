#pragma once

#include "../Events/EventListener.h"

namespace Ledlib {

class RemoteSfxEventListener : public EventListener {
public:
	virtual void OnEvent(Event& event);
};

}
