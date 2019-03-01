#include "RemoteSfxEventListener.h"
#include "RemoteSfx.h"
#include "../Events/Event.h"

namespace Ledlib {

void RemoteSfxEventListener::OnEvent(Event& event){
	RemoteSfx::OnEvent(event);
}

}
