#include "LedlibEventHandler.h"
#include "Ledlib.h"
#include "Config.h"
#include "Events/Event.h"
#include "Events/EventManager.h"
#include "Remote/ServerMessage.h"
#include "Remote/ServerManager.h"
#include "Display/DisplayManager.h"
#include "Util/Strings.h"

namespace Ledlib {

void LedlibEventHandler::Subscribe(){
	EventManager::SubscribeMessage("client_connected", this, &LedlibEventHandler::OnMessageClientConnected);
	EventManager::SubscribeMessage("set_brightness", this, &LedlibEventHandler::OnMessageSetBrightness);
	EventManager::SubscribeMessage("launch_app", this, &LedlibEventHandler::OnMessageLaunchApp);
	EventManager::SubscribeMessage("quit_matrix", this, &LedlibEventHandler::OnMessageQuitMatrix);
}

void LedlibEventHandler::Unsubscribe(){
	EventManager::UnsubscribeMessagesAll(this);
}

void LedlibEventHandler::OnMessageClientConnected(void* obj, MessageEvent& event){
	if(!event.fromClient){
		int clientId = event.GetParamInt(0);
		ServerMessage welcomeMessage = ServerMessage("welcome");
		welcomeMessage.AddParam("brightness", DisplayManager::GetBrightness());
		welcomeMessage.AddParam("app", Strings::GetAppName());
		ServerManager::SendMessage(welcomeMessage, clientId);
	}
}
void LedlibEventHandler::OnMessageSetBrightness(void *obj, MessageEvent &event){
	int brightness = event.GetParamInt(0);
	DisplayManager::SetBrightness(brightness);
	Config::Set("mat_brightness", brightness, Config::Target::User);
	ServerMessage message = ServerMessage("set_brightness");
	message.AddParam("brightness", brightness);
	ServerManager::SendMessage(message);
}
void LedlibEventHandler::OnMessageLaunchApp(void *obj, MessageEvent &event){
	std::string appName = event.GetParam(0);
	LedMatrixLibrary::LaunchApp(appName.c_str());
}
void LedlibEventHandler::OnMessageQuitMatrix(void *obj, MessageEvent &event){
	LedMatrixLibrary::RequestExit();
}

}
