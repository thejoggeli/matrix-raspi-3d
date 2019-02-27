#include "Ledlib.h"
#include "Log.h"
#include "Config.h"
#include "Time.h"
#include "Math/Numbers.h"
#include "Util/Strings.h"
#include "Display/DisplayManager.h"
#include "Events/Event.h"
#include "Events/EventManager.h"
#include "Events/EventListener.h"
#include "Remote/ServerManager.h"
#include "Remote/ServerMessage.h"
#include "Remote/ClientManager.h"
#include "Remote/Client.h"

namespace Ledlib {

int LedMatrixLibrary::initCounter = 0;
int LedMatrixLibrary::startCounter = 0;
int LedMatrixLibrary::exitCounter = 0;

class LedlibEventListener : EventListener {
public:
	void OnEvent(Event& event);
};

void LedlibEventListener::OnEvent(Event& event){
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
		//	MatLib::LaunchApp(appName);
		} else if (messageEvent.message == "quit"){
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

static LedlibEventListener eventListener;

bool LedMatrixLibrary::exitRequested = false;

bool LedMatrixLibrary::Init(){
	if(++initCounter > 1) return false;

	if(!Config::Init()){
		Log(LOG_ERROR, "Ledlib", "Config initialization failed");
	}
	if(!LogLevels::Init()){
		Log(LOG_ERROR, "Ledlib", "LogLevels initialization failed");
		return false;
	}
	if(!Numbers::Init()){
		Log(LOG_ERROR, "Ledlib", "Numbers initialization failed");
		return false;
	}
	if(!DisplayManager::Init()){
		Log(LOG_ERROR, "Ledlib", "DisplayManager initialization failed");
		return false;
	}
	if(!EventManager::Init()){
		Log(LOG_ERROR, "Ledlib", "EventManager initialization failed");
		return false;
	}
	if(!ClientManager::Init()){
		Log(LOG_ERROR, "Ledlib", "ClientManager initialization failed");
		return false;
	}
	if(!ServerManager::Init()){
		Log(LOG_ERROR, "Ledlib", "ServerManager initialization failed");
		return false;
	}
	return true;
}
void LedMatrixLibrary::Start(){
	if(++startCounter > 1) return;
	if(initCounter < 1){
		Log(LOG_ERROR, "Ledlib", "Not initialized");
		return;
	}
	Time::Start();
}
void LedMatrixLibrary::Update(){
	// poll network input
	ServerManager::Poll();
	// clear events from last tick
	for(auto const &client: ClientManager::GetAllCients()){
		client->NextGeneration();
	}
	EventManager::NextGeneration();
	// update clients
	ClientManager::Update();
	// fire events
	EventManager::Update();
	DisplayManager::Sync();

}
void LedMatrixLibrary::Exit(){
	if(++exitCounter > 1) return;
	Log(LOG_INFO, "Ledlib", "Shutting down ...");
	DisplayManager::Shutdown();
	ServerManager::Shutdown();
	Config::WriteFile(Config::Target::App);
	Config::WriteFile(Config::Target::User);
}
void LedMatrixLibrary::RequestExit(){
	exitRequested = true;
}

}
