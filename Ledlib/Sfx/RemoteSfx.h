#pragma once

#include <string>

namespace Ledlib {

class MessageEvent;

class RemoteSfx {
private:
	static int initCounter;
public:
	static const float defaultVolume;
	static bool autoplayMusic;
	static bool Init();
	static void AddFile(const std::string& name, const std::string& file, float volume = defaultVolume, float start = 0, float end = 0);
	static void PlaySound(int clientId, const std::string& name, float volume = defaultVolume, float start = 0,	float end = 0);
	static void StartMusic(int clientId, const std::string& name, float volume = defaultVolume, float start = 0, float end = 0);
	static void StopMusic(int clientId);
	static void OnMessageClientConnected(void* obj, MessageEvent& event);
};

}

