#include <iostream>
#include <cmath>

#include "Ledlib/Config.h"
#include "Ledlib/Log.h"
#include "Ledlib/Events/Event.h"
#include "Ledlib/Events/EventManager.h"
#include "Ledlib2d/App.h"
#include "Ledlib2d/Gfx/Bitmaps.h"
#include "Ledlib2d/Resources/ResourceManager.h"
#include "Ledlib2d/Resources/Bitmap.h"

using namespace std;
using namespace Ledlib;

std::shared_ptr<Bitmap> canvas;

class PainterApp : public App {
	void OnSetup() override;
	void OnStart() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnExit() override;
	static void OnPixelMessage(void* obj, MessageEvent& message);
};
void PainterApp::OnSetup() {
	canvas = Bitmap::CreateEmpty(64, 32);
	Bitmap* hi = ResourceManager::LoadBitmapPng("hi", "painter/hi.png");
	if(hi){
		for(int i = 0; i < canvas->size; i++){
			canvas->image[i] = hi->image[i];
		}
	} else {
		for(int i = 0; i < canvas->size; i+=4){
			canvas->image[i+0] = (uint8_t)i;
			canvas->image[i+1] = (uint8_t)(255-i);
			canvas->image[i+2] = 127;
			canvas->image[i+3] = 255;
		}
	}
	EventManager::SubscribeMessage("paint", this, &(PainterApp::OnPixelMessage));
}
void PainterApp::OnStart(){
}
void PainterApp::OnUpdate() {
}
void PainterApp::OnRender() {
	canvas->Update();
	Gfx::DrawBitmap(canvas.get(), 0, 0);
}
void PainterApp::OnExit(){
	EventManager::UnsubscribeMessagesAll(this);
}
void PainterApp::OnPixelMessage(void* obj, MessageEvent& message){
	PainterApp* painter = (PainterApp*) obj;
	unsigned int rgb = message.GetParamInt(0);
	int x = message.GetParamInt(1);
	int y = message.GetParamInt(2);
	canvas->SetPixel(x, y, (rgb>>16)&0xFF, (rgb>>8)&0xFF, rgb&0xFF);
}

int main(){
	App::Run(new PainterApp());
	return EXIT_SUCCESS;
}
