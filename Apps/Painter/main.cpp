#include <iostream>
#include <cmath>
#include <algorithm>
#include <sys/stat.h>
#include <time.h>

#include "Ledlib/Config.h"
#include "Ledlib/Log.h"
#include "Ledlib/Time.h"
#include "Ledlib/Util/Lodepng.h"
#include "Ledlib/Util/Timer.h"
#include "Ledlib/Display/DisplayManager.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Events/Event.h"
#include "Ledlib/Events/EventManager.h"
#include "Ledlib/Remote/ServerMessage.h"
#include "Ledlib/Remote/ServerManager.h"
#include "Ledlib2d/App.h"
#include "Ledlib2d/Gfx/Bitmaps.h"
#include "Ledlib2d/Gfx/Renderer.h"
#include "Ledlib2d/Resources/ResourceManager.h"
#include "Ledlib2d/Resources/Bitmap.h"

using namespace std;
using namespace Ledlib;

static const int maxUndoSteps = 32;
static int undoStepsAvailable = 0;
static int redoStepsAvailable = 0;
static int undoPointer = maxUndoSteps-1;
static std::vector<uint8_t*> undoPixels;
static std::shared_ptr<Bitmap> canvas;
static const char* digits = "0123456789ABCDEF";
static Timer saveTimer;
static float saveInterval = 10.0f;
static std::string autosavePath;
static std::string manualsavePath;

class PainterApp : public App {
	void OnSetup() override;
	void OnStart() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnExit() override;
	static void AutosaveTrigger();
	static void SavePNG(int clientId);
	static void PushUndo();
	static void PopUndo();
	static void PopRedo();
	static void SendPixels(int clientId);
	static void OnMessagePixel(void* obj, MessageEvent& message);
	static void OnMessagePixels(void* obj, MessageEvent& message);
	static void OnMessagePixelsChunk(void* obj, MessageEvent& message);
	static void OnMessageRequestPixels(void* obj, MessageEvent& message);
	static void OnMessageUndo(void* obj, MessageEvent& message);
	static void OnMessageRedo(void* obj, MessageEvent& message);
	static void OnMessageSaveImage(void* obj, MessageEvent& message);
};
void PainterApp::OnSetup() {
	canvas = Bitmap::CreateEmpty(DisplayManager::width, DisplayManager::height);
	for(int i = 0; i < maxUndoSteps; i++){
		uint8_t* pixels_ptr = new uint8_t[canvas->size];
		undoPixels.push_back(pixels_ptr);
	}
//	Bitmap* hi = ResourceManager::LoadBitmapPng("hi", "painter/hi.png");
	Bitmap* hi = nullptr;
	if(hi){
		for(int i = 0; i < canvas->size; i++){
			canvas->image[i] = hi->image[i];
		}
	} else {
		for(int i = 0; i < canvas->size; i+=4){
			canvas->image[i+0] = 0;
			canvas->image[i+1] = 0;
			canvas->image[i+2] = 0;
			canvas->image[i+3] = 255;
		}
	}
	EventManager::SubscribeMessage("pixel", this, &PainterApp::OnMessagePixel);
	EventManager::SubscribeMessage("pixels", this, &PainterApp::OnMessagePixels);
	EventManager::SubscribeMessage("pixels_chunk", this, &PainterApp::OnMessagePixelsChunk);
	EventManager::SubscribeMessage("request_pixels", this, &PainterApp::OnMessageRequestPixels);
	EventManager::SubscribeMessage("undo", this, &PainterApp::OnMessageUndo);
	EventManager::SubscribeMessage("redo", this, &PainterApp::OnMessageRedo);
	EventManager::SubscribeMessage("save_image", this, &PainterApp::OnMessageSaveImage);
	Gfx::SetAutoClear(false);

	std::string web_painter_path = LEDLIB_WEB_PATH;
	web_painter_path += "/painter/";
	mkdir(web_painter_path.c_str(),  ACCESSPERMS);

	std::string web_manualsave_path = LEDLIB_WEB_PATH;
	web_manualsave_path += "/painter/save/";
	mkdir(web_manualsave_path.c_str(), ACCESSPERMS);

	std::string web_autosave_path = LEDLIB_WEB_PATH;
	web_autosave_path += "/painter/autosave/";
	mkdir(web_autosave_path.c_str(), ACCESSPERMS);

	std::string web_autosave_daily_path = LEDLIB_WEB_PATH;
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime (&rawtime);
	char timeBuffer[32];
	strftime(timeBuffer,sizeof(timeBuffer),"%Y-%m-%d",timeinfo);
	web_autosave_daily_path += "/painter/autosave/";
	web_autosave_daily_path += timeBuffer;
	web_autosave_daily_path += "/";
	mkdir(web_autosave_daily_path.c_str(), ACCESSPERMS);

	Log(web_autosave_daily_path);

	Log(LOG_INFO, "Painter", iLog << "daily dir is " << timeBuffer);

	manualsavePath = web_manualsave_path;
	autosavePath = web_autosave_daily_path;

}
void PainterApp::OnStart(){
}
void PainterApp::OnUpdate() {
	if(saveTimer.IsFinished()){
		saveTimer.Reset();
		PainterApp::SavePNG(0);
	}
}
void PainterApp::OnRender() {
	canvas->Update();
	Gfx::DrawBitmap(canvas.get(), 0, 0);
}
void PainterApp::OnExit(){
	for(int i = 0; i < maxUndoSteps; i++){
		delete[] undoPixels[i];
	}
	undoPixels.clear();
	EventManager::UnsubscribeMessagesAll(this);
}
void PainterApp::OnMessageSaveImage(void *obj, MessageEvent &message){
	SavePNG(message.clientId);
}
void PainterApp::AutosaveTrigger(){
	if(!saveTimer.IsRunning()){
		saveTimer.Start(saveInterval);
		Log(LOG_INFO, "Painter", iLog << "autosave PNG in " << saveInterval << " seconds");
	}
}
void PainterApp::SavePNG(int clientId){
	if(clientId == 0){
		uint64_t time_start = Time::GetRealTimeMillisecondsInt();
		std::string name = std::to_string(Time::GetRealTimeSecondsInt()) + ".png";
		lodepng::encode(autosavePath+name, canvas->image, canvas->width, canvas->height);
		uint64_t duration = Time::GetRealTimeMillisecondsInt()-time_start;
		Log(LOG_INFO, "Painter", iLog << "autosave PNG as " << name << " (" << duration << "ms)");
	} else {
		uint64_t time_start = Time::GetRealTimeMillisecondsInt();
		std::string name = std::to_string(Time::GetRealTimeSecondsInt());
		name += "_" + std::to_string(clientId) + ".png";
		lodepng::encode(manualsavePath+name, canvas->image, canvas->width, canvas->height);
		uint64_t duration = Time::GetRealTimeMillisecondsInt()-time_start;
		Log(LOG_INFO, "Painter", iLog << "save PNG as " << name << " (" << duration << "ms) (client=" << clientId << ")");
	}
}
void PainterApp::PushUndo(){
	// point to next undo bin
	undoPointer++;
	if(undoPointer >= maxUndoSteps){
		undoPointer = 0;
	}
	// copy pixels from canvas buffer to undo bin
	std::copy(canvas->image, canvas->image+canvas->size, undoPixels[undoPointer]);
	// update available
	redoStepsAvailable = 0;
	if(undoStepsAvailable < maxUndoSteps-1){
		undoStepsAvailable++;
	}
	Log(LOG_INFO, "Painter", iLog << "PushUndo / Pointer=" << undoPointer << " / Undo=" << undoStepsAvailable << " / Redo=" << redoStepsAvailable);
}
void PainterApp::PopUndo(){
	if(undoStepsAvailable < 1){
		Log(LOG_INFO, "Painter", iLog << "PopUndo / Nothing to pop");
		return;
	}
	// if first undo, push current state to make redo possible
	if(redoStepsAvailable == 0){
		// point to next undo bin
		int pushUndoPointer = undoPointer+1;
		if(pushUndoPointer >= maxUndoSteps){
			pushUndoPointer = 0;
		}
		// copy pixels from canvas buffer to undo bin
		std::copy(canvas->image, canvas->image+canvas->size, undoPixels[pushUndoPointer]);
	}
	// update available
	undoStepsAvailable--;
	redoStepsAvailable++;
	// copy pixels from undo bin to canvas buffer
	std::copy(undoPixels[undoPointer], undoPixels[undoPointer]+canvas->size, canvas->image);
	// point to previous undo bin
	undoPointer--;
	if(undoPointer < 0){
		undoPointer = maxUndoSteps-1;
	}
	Log(LOG_INFO, "Painter", iLog << "PushUndo / Pointer=" << undoPointer << " / Undo=" << undoStepsAvailable << " / Redo=" << redoStepsAvailable);
}
void PainterApp::PopRedo(){
	if(redoStepsAvailable < 1){
		Log(LOG_INFO, "Painter", iLog << "PopRedo / Nothing to pop");
		return;
	}
	// point to next undo bin
	undoPointer++;
	if(undoPointer >= maxUndoSteps){
		undoPointer = 0;
	}
	// update available
	redoStepsAvailable--;
	undoStepsAvailable++;
	// copy pixels from redo bin to canvas buffer
	int redoPointer = undoPointer+1;
	if(redoPointer >= maxUndoSteps){
		redoPointer = 0;
	}
	std::copy(undoPixels[redoPointer], undoPixels[redoPointer]+canvas->size, canvas->image);
	Log(LOG_INFO, "Painter", iLog << "PushUndo / Pointer=" << undoPointer << " / Undo=" << undoStepsAvailable << " / Redo=" << redoStepsAvailable);
}
void PainterApp::OnMessagePixel(void *obj, MessageEvent &message){
	bool save = message.GetParamInt(0);
	if(save){
		PainterApp::PushUndo();
	}
	unsigned int rgb = message.GetParamInt(1);
	int x = message.GetParamInt(2);
	int y = message.GetParamInt(3);
	canvas->SetPixelBytes(x, y, (rgb>>16)&0xFF, (rgb>>8)&0xFF, rgb&0xFF);
	// respond
	ServerMessage smsg = ServerMessage("pixel");
	std::string rgbhex(7, '0');
	rgbhex[6] = 'h';
	rgbhex[0] = digits[(rgb >> 20)&0xF];
	rgbhex[1] = digits[(rgb >> 16)&0xF];
	rgbhex[2] = digits[(rgb >> 12)&0xF];
	rgbhex[3] = digits[(rgb >> 8)&0xF];
	rgbhex[4] = digits[(rgb >> 4)&0xF];
	rgbhex[5] = digits[(rgb >> 0)&0xF];
	smsg.AddParam("rgb", rgbhex);
	smsg.AddParam("x", x);
	smsg.AddParam("y", y);
	smsg.AddParam("undo", undoStepsAvailable);
	smsg.AddParam("redo", redoStepsAvailable);
	ServerManager::SendMessage(smsg);
	PainterApp::AutosaveTrigger();
}
void PainterApp::OnMessagePixels(void *obj, MessageEvent &message){
	PainterApp::PushUndo();
	int i = 0;
	for(int x = 0; x < 64; x++){
		for(int y = 0; y < 32; y++){
			unsigned int rgb = message.GetParamInt(i++);
			canvas->SetPixelBytes(x, y, (rgb>>16)&0xFF, (rgb>>8)&0xFF, rgb&0xFF);
		}
	}
	// send pixels to all clients
	ServerMessage smsg = ServerMessage("pixels");
	int hex_len = canvas->width * canvas->height * 6 + 1;
	std::string str(hex_len, '0');
	str[hex_len-1] = 'h';
	int j = 0;
	for (int i = 0; i < canvas->size; i += 4){
		str[j++] = digits[(canvas->image[i+0] >> 4)&0xF];
		str[j++] = digits[(canvas->image[i+0] >> 0)&0xF];
		str[j++] = digits[(canvas->image[i+1] >> 4)&0xF];
		str[j++] = digits[(canvas->image[i+1] >> 0)&0xF];
		str[j++] = digits[(canvas->image[i+2] >> 4)&0xF];
		str[j++] = digits[(canvas->image[i+2] >> 0)&0xF];
	}
	smsg.AddParam("rgb", str);
	smsg.AddParam("undo", undoStepsAvailable);
	smsg.AddParam("redo", redoStepsAvailable);
	ServerManager::SendMessage(smsg);
	PainterApp::AutosaveTrigger();
}
void PainterApp::OnMessagePixelsChunk(void *obj, MessageEvent &message){
	bool save = message.GetParamInt(0);
	if(save){
		PainterApp::PushUndo();
	}
	int x0 = message.GetParamInt(1);
	int y0 = message.GetParamInt(2);
	int w0 = message.GetParamInt(3);
	int h0 = message.GetParamInt(4);
	int w = x0+w0;
	int h = y0+h0;
	// prepare pixels_chunk to all clients
	ServerMessage smsg = ServerMessage("pixels_chunk");
	int hex_len = w0*h0*6+1;
	std::string str(hex_len, '-');
	str[hex_len-1] = 'h';
	// apply chunk
	int i = 5;
	int j = 0;
	const int align_step = w0*6;
	const int align_step2 = w0*h0*6;
	for(int x = x0; x < w; x++){
		for(int y = y0; y < h; y++){
			if(message.GetParam(i) != "-"){
				unsigned int rgb = message.GetParamInt(i);
				canvas->SetPixelBytes(x, y, (rgb>>16)&0xFF, (rgb>>8)&0xFF, rgb&0xFF);
				str[j+0] = digits[(rgb >> 20)&0xF];
				str[j+1] = digits[(rgb >> 16)&0xF];
				str[j+2] = digits[(rgb >> 12)&0xF];
				str[j+3] = digits[(rgb >> 8)&0xF];
				str[j+4] = digits[(rgb >> 4)&0xF];
				str[j+5] = digits[(rgb >> 0)&0xF];
			}
			i += 1;
			j += align_step;
		}
		j -= align_step2;
		j += 6;
	}
	smsg.AddParam("rgb", str);
	smsg.AddParam("x", x0);
	smsg.AddParam("y", y0);
	smsg.AddParam("w", w0);
	smsg.AddParam("h", h0);
	smsg.AddParam("undo", undoStepsAvailable);
	smsg.AddParam("redo", redoStepsAvailable);
	ServerManager::SendMessage(smsg);
	PainterApp::AutosaveTrigger();
}
void PainterApp::OnMessageRequestPixels(void *obj, MessageEvent &message){
	SendPixels(message.clientId);
}
void PainterApp::OnMessageUndo(void *obj, MessageEvent &message){
	PainterApp::PopUndo();
	SendPixels(0);
	PainterApp::AutosaveTrigger();
}
void PainterApp::OnMessageRedo(void *obj, MessageEvent &message){
	PainterApp::PopRedo();
	SendPixels(0);
	PainterApp::AutosaveTrigger();
}
void PainterApp::SendPixels(int clientId){
	ServerMessage smsg = ServerMessage("pixels");
	int hex_len = canvas->width * canvas->height * 6 + 1;
	std::string str(hex_len, '0');
	str[hex_len-1] = 'h';
	int j = 0;
	for (int i = 0; i < canvas->size; i += 4){
		str[j++] = digits[(canvas->image[i+0] >> 4)&0xF];
		str[j++] = digits[(canvas->image[i+0] >> 0)&0xF];
		str[j++] = digits[(canvas->image[i+1] >> 4)&0xF];
		str[j++] = digits[(canvas->image[i+1] >> 0)&0xF];
		str[j++] = digits[(canvas->image[i+2] >> 4)&0xF];
		str[j++] = digits[(canvas->image[i+2] >> 0)&0xF];
	}
	smsg.AddParam("rgb", str);
	smsg.AddParam("undo", undoStepsAvailable);
	smsg.AddParam("redo", redoStepsAvailable);
	ServerManager::SendMessage(smsg, clientId);
}

int main(){
	App::Run(new PainterApp());
	return EXIT_SUCCESS;
}
