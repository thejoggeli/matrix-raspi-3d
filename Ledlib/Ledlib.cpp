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
#include "Remote/ServerMessage.h"
#include "Remote/ServerManager.h"
#include "Remote/ClientManager.h"
#include "Remote/Client.h"
#include "Sfx/RemoteSfx.h"
#include "sys/sysinfo.h"

using namespace std;

namespace Ledlib {

int LedMatrixLibrary::initCounter = 0;
int LedMatrixLibrary::startCounter = 0;
int LedMatrixLibrary::exitCounter = 0;

bool LedMatrixLibrary::exitRequested = false;

static LedlibEventHandler eventHandler;
static int fps;
static Timer statusTimer;
static int fpsCounter = 0;
static int event_counter = 0;
static float fpsInterval = 5.0f;
static float system_temperature = 0.0f;
static long long system_clock = 0;
static long long system_memory_total = 0;
static long long system_memory_used = 0;
static ServerMessage statusServerMessage("status");
static std::string statusLogMessage;

bool LedMatrixLibrary::Init(){
	if(++initCounter > 1) return false;
	// components
	if(!Config::Init()){
		Log(LOG_ERROR, "Ledlib", "Config initialization failed");
		return false;
	}
	// log levels
	if(!LogLevels::Init()){
		Log(LOG_ERROR, "Ledlib", "LogLevels initialization failed");
		return false;
	}
	// numbers
	if(!Numbers::Init()){
		Log(LOG_ERROR, "Ledlib", "Numbers initialization failed");
		return false;
	}
	// display manager
	if(!DisplayManager::Init()){
		Log(LOG_ERROR, "Ledlib", "DisplayManager initialization failed");
		return false;
	}
	// event manager
	if(!EventManager::Init()){
		Log(LOG_ERROR, "Ledlib", "EventManager initialization failed");
		return false;
	}
	// client manager
	if(!ClientManager::Init()){
		Log(LOG_ERROR, "Ledlib", "ClientManager initialization failed");
		return false;
	}
	// server manager
	if(!ServerManager::Init()){
		Log(LOG_ERROR, "Ledlib", "ServerManager initialization failed");
		return false;
	}
	// ledlib event handler
	Log(LOG_INFO, "Ledlib", "Initializing LedlibEventHandler");
	eventHandler.Subscribe();
	// system sounds
	RemoteSfx::AddFile("system_quit", "system/quit.mp3");
	return true;
}
void LedMatrixLibrary::Start(){
	if(++startCounter > 1) return;
	if(initCounter < 1){
		Log(LOG_ERROR, "Ledlib", "Not initialized");
		return;
	}
	Time::Start();
	statusTimer.Start(fpsInterval);
}
void LedMatrixLibrary::Update(){
	// poll network input
	ServerManager::Poll();
	// clear events from last tick
	for(auto const &client: ClientManager::GetAllCients()){
		client->NextGeneration();
	}
	EventManager::NextGeneration();
	// update clients (set new input states etc)
	ClientManager::Update();
	// fire events
	EventManager::Update();
	// time
	Time::Update();
	// fps counter
	fpsCounter++;
	if(statusTimer.IsFinished()){
		fps = static_cast<int>(static_cast<float>(fpsCounter)/fpsInterval);
		ServerMessage serverMessage = ServerMessage("status");
		event_counter = EventManager::eventCounterTemp;
		EventManager::eventCounterTemp = 0;
		statusTimer.Restart();
		fpsCounter = 0;
	}
	// staggered status update to prevent possible flicker
	switch(fpsCounter){
	case 1: {
		if(Config::GetBool("is_rpi")){
			// poll temperature
			FILE *fp = popen("vcgencmd measure_temp", "r");
			if (fp == nullptr){
				system_temperature = 0.0f;
			} else {
				fscanf(fp, "temp=%f'C", &system_temperature);
			}
			pclose(fp);
		} else {
			system_temperature = 0.0f;
		}
		break;
	}
	case 2: {
		// poll clock
		FILE *fp = popen("cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq", "r");
		if (fp == nullptr){
			system_clock = 0.0f;
		} else {
			fscanf(fp, "%lld", &system_clock);
		}
		pclose(fp);
		break;
	}
	case 3: {
		// poll memory
		struct sysinfo memInfo;
		sysinfo (&memInfo);
		system_memory_total = memInfo.totalram;
		system_memory_total *= memInfo.mem_unit;
		system_memory_used = memInfo.totalram - memInfo.freeram;
		system_memory_used *= memInfo.mem_unit;
		break;
	}
	case 4:
		// print log message
		Log(LOG_INFO, "Ledlib", iLog
			<< "Fps=" << fps << " / "
			<< "Dt=" << Time::deltaTime << " / "
			<< "Clients=" << ClientManager::GetAllCients().size() << " / "
			<< "Connections=" << ServerManager::GetNumConnections() << " / "
			<< "Events=" << EventManager::eventCounterTemp << "/" << EventManager::eventCounterTotal
		);
		Log(LOG_INFO, "Ledlib", iLog
			<< "Temp=" << system_temperature << " / "
			<< "Clock=" << system_clock << " / "
			<< "Memory=" << (system_memory_used/1000000L) << "M/" << (system_memory_total/1000000L) << "M"
		);;
		break;
	case 5:
		// prepare server message
		statusServerMessage = ServerMessage("status");
		statusServerMessage.AddParam("fps", fps);
		statusServerMessage.AddParam("clients", ClientManager::GetAllCients().size());
		statusServerMessage.AddParam("temp", system_temperature);
		statusServerMessage.AddParam("clock", system_clock);
		statusServerMessage.AddParam("mem_used", system_memory_used);
		statusServerMessage.AddParam("mem_total", system_memory_total);
		break;
	case 6:
		// send server message
		ServerManager::SendMessage(statusServerMessage);
		break;
	default:
		break;
	}
}
void LedMatrixLibrary::Render(){
	DisplayManager::Sync();
}
void LedMatrixLibrary::Exit(){
	if(++exitCounter > 1) return;
	Log(LOG_INFO, "Ledlib", "Shutting down ...");
	eventHandler.Unsubscribe();
	DisplayManager::Shutdown();
	ServerManager::Shutdown();
	Config::WriteFile(Config::Target::App);
	Config::WriteFile(Config::Target::User);
}
void LedMatrixLibrary::RequestExit(){
	Log(LOG_INFO, "Ledlib", "Shutdown requested");
	exitRequested = true;
	std::vector<std::string> args;
	args.push_back("quit_matrix");
	Config::SetLauncherArgs(args);
	RemoteSfx::PlaySound(0, "system_quit");
}

void LedMatrixLibrary::LaunchApp(const char* name){
	if(!exitRequested){
		exitRequested = true;
		std::vector<std::string> args;
		args.push_back(name);
		Config::SetLauncherArgs(args);
	}
}

}
