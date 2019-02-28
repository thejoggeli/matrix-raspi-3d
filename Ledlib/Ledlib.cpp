#include "Ledlib.h"
#include "LedlibEventHandler.h"
#include "Log.h"
#include "Config.h"
#include "Time.h"
#include "Math/Numbers.h"
#include "Util/Strings.h"
#include "Util/Timer.h"
#include "Display/DisplayManager.h"
#include "Events/EventManager.h"
#include "Remote/ServerManager.h"
#include "Remote/ClientManager.h"
#include "Remote/Client.h"

using namespace std;

namespace Ledlib {

int LedMatrixLibrary::initCounter = 0;
int LedMatrixLibrary::startCounter = 0;
int LedMatrixLibrary::exitCounter = 0;

bool LedMatrixLibrary::exitRequested = false;

static shared_ptr<LedlibEventHandler> eventHandler = nullptr;
static Timer fpsTimer;
static int fpsCounter = 0;
static float fpsInterval = 5.0f;

bool LedMatrixLibrary::Init(){
	if(++initCounter > 1) return false;
	// event handler
	Log(LOG_INFO, "Ledlib", "Initializing LedlibEventHandler");
	eventHandler = make_shared<LedlibEventHandler>();
	eventHandler->StartListening();
	// components
	if(!Config::Init()){
		Log(LOG_ERROR, "Ledlib", "Config initialization failed");
		return false;
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
	fpsTimer.Start(fpsInterval);
}
void LedMatrixLibrary::Update(){
	// poll network input
	ServerManager::Poll();
	// time
	Time::Update();
	// fps counter
	fpsCounter++;
	if(fpsTimer.IsFinished()){
		int fps = static_cast<int>(static_cast<float>(fpsCounter)/fpsInterval);
		Log(LOG_INFO, "Matlib", iLog
			<< "Fps: " << fps
			<< " / Clients: " << ClientManager::GetAllCients().size()
			<< " / Connections: " << ServerManager::GetNumConnections()
			<< " / Events: " << EventManager::GetAllEvents().size()
		);

		fpsTimer.Restart();
		fpsCounter = 0;
	}
	// clear events from last tick
	for(auto const &client: ClientManager::GetAllCients()){
		client->NextGeneration();
	}
	EventManager::NextGeneration();
	// update clients (set new input states etc)
	ClientManager::Update();
	// fire events
	EventManager::Update();
}
void LedMatrixLibrary::Render(){
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
	Log(LOG_INFO, "Ledlib", "Shutdown requested");
	exitRequested = true;
}

}
