#include "RemoteSfx.h"
#include "../Log.h"
#include "../Remote/ServerManager.h"
#include "../Remote/ServerMessage.h"
#include "../Events/EventManager.h"
#include "../Events/Event.h"

using namespace std;

namespace Ledlib {

static ServerMessage entryMessage = ServerMessage("load_sfx");
static unordered_map<string, string> namesMap;
static string currentMusicName;
static bool musicPlaying = false;

static int messageHandle;

int RemoteSfx::initCounter = 0;
const float RemoteSfx::defaultVolume = 0.5f;
bool RemoteSfx::autoplayMusic = true;

bool RemoteSfx::Init(){
	if(++initCounter > 1) return false;
	Log(LOG_INFO, "RemoteSfx", "Initializing");
	EventManager::SubscribeMessage("client_connected", &messageHandle, &RemoteSfx::OnMessageClientConnected);
	return true;
}

void RemoteSfx::AddFile(const string& name, const string& file, float volume, float start, float end){
	if(initCounter < 1) Init();
	namesMap[name] = file;
	entryMessage.AddParam("files", file);
	entryMessage.AddParam("start", start);
	entryMessage.AddParam("end", end);
	entryMessage.AddParam("volume", volume);
	Log(LOG_DEBUG, "RemoteSfx", iLog << "Audio file added: " << file);
}
void RemoteSfx::PlaySound(int clientId, const string& name, float volume, float start, float end){
	if(initCounter < 1) Init();
	ServerMessage msg = ServerMessage("play_sound");
	msg.AddParam("file", namesMap[name]);
	if(volume != defaultVolume) msg.AddParam("volume", volume);
	if(start != 0.0f) msg.AddParam("start", start);
	if(end != 0.0f) msg.AddParam("end", end);
	msg.AddParam("end", end);
	ServerManager::SendMessage(msg, clientId);
}
void RemoteSfx::StartMusic(int clientId, const string& name, float volume, float start, float end){
	if(initCounter < 1) Init();
	currentMusicName = name;
	ServerMessage msg = ServerMessage("start_music");
	msg.AddParam("file", namesMap[name]);
	if(volume != defaultVolume) msg.AddParam("volume", volume);
	if(start != 0.0f) msg.AddParam("start", start);
	if(end != 0.0f) msg.AddParam("end", end);
	ServerManager::SendMessage(msg, clientId);
	musicPlaying = true;
}
void RemoteSfx::StopMusic(int clientId){
	if(initCounter < 1) Init();
	ServerMessage msg = ServerMessage("stop_music");
	ServerManager::SendMessage(msg, clientId);
	musicPlaying = false;
}

void RemoteSfx::OnMessageClientConnected(void *obj, MessageEvent &event){
	ServerManager::SendMessage(entryMessage, event.clientId);
	if(autoplayMusic && musicPlaying){
		StartMusic(event.clientId, currentMusicName);
	}
}

}


