#pragma once

namespace Ledlib {

class MessageEvent;
class Event;

class LedlibEventHandler {
public:
	void Subscribe();
	void Unsubscribe();
	static void OnMessageClientConnected(void* obj, MessageEvent& event);
	static void OnMessageSetBrightness(void* obj, MessageEvent& event);
	static void OnMessageLaunchApp(void* obj, MessageEvent& event);
	static void OnMessageQuitMatrix(void* obj, MessageEvent& event);
};

}
