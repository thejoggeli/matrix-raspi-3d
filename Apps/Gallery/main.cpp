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

class WelcomeApp : public App {
	shared_ptr<Bitmap> pixels;
	void OnSetup() override;
	void OnStart() override;
	void OnUpdate() override;
	void OnRender() override;
	static void OnMessageClientConnected(void* obj, MessageEvent& event);
};
void WelcomeApp::OnMessageClientConnected(void* obj, MessageEvent &event){

}
void WelcomeApp::OnSetup() {
	EventManager::SubscribeMessage("client_connected", this, &WelcomeApp::OnMessageClientConnected);
	ResourceManager::LoadFontBdf("font-1", "7x13B.bdf");
	ResourceManager::LoadFontBdf("font-2", "5x8.bdf");
}
void WelcomeApp::OnStart(){

}
void WelcomeApp::OnUpdate() {

}
void WelcomeApp::OnRender() {

	BaseFont* font_1 = ResourceManager::GetFont("font-1");
	BaseFont* font_2 = ResourceManager::GetFont("font-2");

	Gfx::SetFont(font_1);
	Gfx::SetTextColor(1.0f, 1.0f, 0.0f);
	Gfx::DrawText("Gallery", 0, 5);

	Gfx::SetFont(font_2);
	Gfx::SetTextColor(1.0f, 1.0f, 1.0f);
	Gfx::DrawText("work in progress", sin(Time::sinceStart)*12, -5);

}

int main(){
	App::Run(new WelcomeApp());
	return EXIT_SUCCESS;
}
