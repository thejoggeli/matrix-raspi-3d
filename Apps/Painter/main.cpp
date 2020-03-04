#include <iostream>
#include <cmath>

#include "Ledlib/Config.h"
#include "Ledlib/Log.h"
#include "Ledlib/Time.h"
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

std::shared_ptr<Bitmap> canvas;
static const char* digits = "0123456789ABCDEF";
float lastInputTime = 0.0f;

class PainterApp : public App {
	void OnSetup() override;
	void OnStart() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnExit() override;
	static void OnMessagePixel(void* obj, MessageEvent& message);
	static void OnMessagePixels(void* obj, MessageEvent& message);
	static void OnMessagePixelsChunk(void* obj, MessageEvent& message);
	static void OnMessageRequestPixels(void* obj, MessageEvent& message);
};
void PainterApp::OnSetup() {
	canvas = Bitmap::CreateEmpty(64, 32);
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
	Gfx::SetAutoClear(false);
}
void PainterApp::OnStart(){
}
void PainterApp::OnUpdate() {
}
void PainterApp::OnRender() {
	canvas->Update();
//	float alpha = (Time::sinceStart - lastInputTime) > 1.0f ? 1.0f : 0.015f;
//	Gfx::SetBitmapColor(1, 1, 1, alpha);
	Gfx::DrawBitmap(canvas.get(), 0, 0);
}
void PainterApp::OnExit(){
	EventManager::UnsubscribeMessagesAll(this);
}
void PainterApp::OnMessagePixel(void *obj, MessageEvent &message){
	PainterApp* painter = (PainterApp*) obj;
	unsigned int rgb = message.GetParamInt(0);
	int x = message.GetParamInt(1);
	int y = message.GetParamInt(2);
	canvas->SetPixelBytes(x, y, (rgb>>16)&0xFF, (rgb>>8)&0xFF, rgb&0xFF);
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
	ServerManager::SendMessage(smsg);
	lastInputTime = Time::sinceStart;
}
void PainterApp::OnMessagePixels(void *obj, MessageEvent &message){
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
	ServerManager::SendMessage(smsg);
}
void PainterApp::OnMessagePixelsChunk(void *obj, MessageEvent &message){
	int x0 = message.GetParamInt(0);
	int y0 = message.GetParamInt(1);
	int w0 = message.GetParamInt(2);
	int h0 = message.GetParamInt(3);
	int w = x0+w0;
	int h = y0+h0;
	// prepare pixels_chunk to all clients
	ServerMessage smsg = ServerMessage("pixels_chunk");
	int hex_len = w0*h0*6+1;
	std::string str(hex_len, '0');
	str[hex_len-1] = 'h';
	// apply chunk
	int i = 4;
	int j = 0;
	const int align_step = w0*6;
	const int align_step2 = w0*h0*6;
	for(int x = x0; x < w; x++){
		for(int y = y0; y < h; y++){
			unsigned int rgb = message.GetParamInt(i++);
			canvas->SetPixelBytes(x, y, (rgb>>16)&0xFF, (rgb>>8)&0xFF, rgb&0xFF);
			str[j+0] = digits[(rgb >> 20)&0xF];
			str[j+1] = digits[(rgb >> 16)&0xF];
			str[j+2] = digits[(rgb >> 12)&0xF];
			str[j+3] = digits[(rgb >> 8)&0xF];
			str[j+4] = digits[(rgb >> 4)&0xF];
			str[j+5] = digits[(rgb >> 0)&0xF];
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
	ServerManager::SendMessage(smsg);
}
void PainterApp::OnMessageRequestPixels(void *obj, MessageEvent &message){
	// send pixels to client
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
	ServerManager::SendMessage(smsg, message.clientId);
}

int main(){
	App::Run(new PainterApp());
	return EXIT_SUCCESS;
}
