#include <iostream>
#include <cmath>

#include "Ledlib/Config.h"
#include "Ledlib2d/App.h"
#include "Ledlib/Time.h"
#include "Ledlib/Util/Timer.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Gfx/Text.h"
#include "Ledlib2d/Gfx/Path.h"
#include "Ledlib2d/Gfx/Transform.h"
#include "Ledlib2d/Gfx/Bitmaps.h"
#include "Ledlib2d/Resources/ResourceManager.h"
#include "Ledlib2d/Resources/Bitmap.h"
#include "Ledlib2d/Resources/Font.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Ledlib/Util/ColorHsl.h"
#include "Ledlib/Events/Event.h"
#include "Ledlib/Events/EventManager.h"
#include "glm/vec3.hpp"
#include "Ledlib/Log.h"
#include "Ledlib/Sfx/RemoteSfx.h"
#include "Ledlib/Remote/ClientManager.h"

using namespace std;
using namespace Ledlib;

float xoff = 0.0f;

class WelcomeApp : public App {
	shared_ptr<Bitmap> pixels;
	void OnSetup() override;
	void OnStart() override;
	void OnUpdate() override;
	void OnRender() override;
	static void OnMessageClientConnected(void* obj, MessageEvent& event);
};
void WelcomeApp::OnMessageClientConnected(void* obj, MessageEvent &event){
	RemoteSfx::PlaySound(event.clientId, "startup");
}
void WelcomeApp::OnSetup() {
	ResourceManager::LoadFontBdf("font-1", "6x10.bdf");
	ResourceManager::LoadBitmapPng("bg", "welcome/bg.png");
//	RemoteSfx::AddFile("hello_0", "welcome/hello_0.wav");
	RemoteSfx::AddFile("startup", "welcome/startup.mp3");
	EventManager::SubscribeMessage("client_connected", this, &WelcomeApp::OnMessageClientConnected);
}
void WelcomeApp::OnStart(){

}
void WelcomeApp::OnUpdate() {

}
void WelcomeApp::OnRender() {

	Gfx::Clear();

//	float h1 = 0.0 + Gfx::height/4.0-2.0;
//	float h2 = 0.0 - Gfx::height/4.0+2.0;

//	Gfx::SetDrawColor(0.1f, 0.0f, 0.0f);
//	Gfx::FillRect(0, 0, Gfx::width, Gfx::height);

	Bitmap* bg = ResourceManager::GetBitmap("bg");
	float aspect = (float)bg->width/(float)bg->height;
	float w = 64;
	float h = w/aspect;
//	Gfx::Save();
//	Gfx::Scale((sin(Time::sinceStart)*0.5+0.5)*0.2+1);
	Gfx::SetBitmapColor(0.35, 0.35, 0.35, 1.0);
	Gfx::DrawBitmap(bg, 0, 0, w, h);
//	Gfx::Restore();

/*	xoff -= Time::deltaTime*10;
	Bitmap* bg = ResourceManager::GetBitmap("bg");
	float aspect = (float)bg->width/(float)bg->height;
	float w = 64;
	float h = w/aspect;

	if(xoff < -w*2){
		xoff += w*2;
	}

	Gfx::Save();
	Gfx::Translate(xoff, 0);

	Gfx::SetBitmapColor(0.35, 0.35, 0.35, 1.0);
	Gfx::DrawBitmap(bg, 0, 0, w, h);

	Gfx::Translate(w, 0);

	Gfx::Save();
	Gfx::Scale(-1, 1);
	Gfx::DrawBitmap(bg, 0, 0, w, h);
	Gfx::Restore();

	Gfx::Translate(w, 0);

	Gfx::Save();
	Gfx::DrawBitmap(bg, 0, 0, w, h);
	Gfx::Restore();

	Gfx::Translate(w, 0);

	Gfx::Save();
	Gfx::Scale(-1, 1);
	Gfx::DrawBitmap(bg, 0, 0, w, h);
	Gfx::Restore();

	Gfx::Restore(); */

	BaseFont* font = ResourceManager::GetFont("font-1");
	Gfx::Save();

	Gfx::SetFont(font);
	Gfx::DrawText("joggeli.net", 0, 0);
	Gfx::Restore();


/*	Gfx::Save();
	float a = Time::sinceStart;
	float x = sin(a) * 64.0f;
	float y = cos(a) * 12.0f;
	Gfx::Translate(x, y);
	Gfx::SetDrawColor(255, 0, 255);
	Gfx::FillRect(0, 0, 6, 6);
	Gfx::Restore(); */

}

int main(){
	App::Run(new WelcomeApp());
	return EXIT_SUCCESS;
}
