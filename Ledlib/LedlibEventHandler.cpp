#include "LedlibEventHandler.h"
#include "Ledlib.h"
#include "Config.h"
#include "Events/Event.h"
#include "Remote/ServerMessage.h"
#include "Remote/ServerManager.h"
#include "Display/DisplayManager.h"
#include "Util/Strings.h"

namespace Ledlib {

void LedlibEventHandler::OnEvent(Event& event){
	// process events
	switch(event.type){
	case EventType::Message: {
		MessageEvent& messageEvent = static_cast<MessageEvent&>(event);
		if(messageEvent.message == "set_brightness"){
			// set brightness
			int brightness = messageEvent.GetParamInt(0);
			DisplayManager::SetBrightness(brightness);
			Config::Set("mat_brightness", brightness, Config::Target::User);
			ServerMessage message = ServerMessage("set_brightness");
			message.AddParam("brightness", brightness);
			ServerManager::SendMessage(message);
		} else if (messageEvent.message == "launch_app"){
			// launch app
			std::string appName = messageEvent.GetParam(0);
			LedMatrixLibrary::LaunchApp(appName.c_str());
		} else if (messageEvent.message == "quit_matrix"){
			// quit
			LedMatrixLibrary::RequestExit();
		}
		break;
	}
	case EventType::ClientConnected: {
		// welcome
		ServerMessage welcomeMessage = ServerMessage("welcome");
		welcomeMessage.AddParam("brightness", DisplayManager::GetBrightness());
		welcomeMessage.AddParam("app", Strings::GetAppName());
		ServerManager::SendMessage(welcomeMessage, event.clientId);
		break;
	}
	default: break;
	}
}

}
