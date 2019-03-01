#include "DisplayManager.h"
#include "Display.h"
#include "MatrixDisplay.h"
#include "DummyDisplay.h"
#include "X11Display.h"
#include "../Config.h"
#include "../Log.h"

namespace Ledlib {

static Display* display;

int DisplayManager::initCounter = 0;
const int& DisplayManager::width = DisplayManager::_width;
const int& DisplayManager::height = DisplayManager::_height;
int DisplayManager::_width = 0;
int DisplayManager::_height = 0;

DisplayManager::DisplayManager(){}

bool DisplayManager::Init(){
	if(++initCounter > 1) return false;
	Log(LOG_INFO, "DisplayManager", "Initializing");
	// display mode
	std::string displayMode = Config::GetString("display_mode");
	if(displayMode == "sim"){
		display = new X11Display();
	} else if(displayMode == "mat"){
		display = new MatrixDisplay();
	} else if(displayMode == "dummy"){
		display = new DummyDisplay();
	} else {
		Log(LOG_ERROR, "DisplayManager", "Invalid display mode");
		return false;
	}
	if(!display->Init()){
		return false;
	}
	_width = display->width;
	_height = display->height;
	return true;
}
void DisplayManager::Sync(){
	display->Sync();
}
int DisplayManager::GetBrightness(){
	return display->GetBrightness();
}
void DisplayManager::SetBrightness(int percent){
	return display->SetBrightness(percent);
}
void DisplayManager::Shutdown(){
	display->Shutdown();
}
void DisplayManager::SetPixelsPointer(void* pointer, int pitch){
	display->pixels = static_cast<uint8_t*>(pointer);
	display->pitch = pitch;
}

}
