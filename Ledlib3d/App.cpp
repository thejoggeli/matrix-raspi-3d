#include "App.h"
#include "AppManager.h"
#include "Gfx.h"
#include "Urho3D/Core/CoreEvents.h"
#include "Ledlib/Ledlib.h"
#include "Ledlib/Display/DisplayManager.h"
#include "Ledlib/Log.h"
#include "Ledlib/Util/Strings.h"
#include "Urho3D/Graphics/Graphics.h"
#include "Urho3D/Graphics/Texture2D.h"
#include "Urho3D/Input/Input.h"

using namespace Urho3D;

namespace Ledlib {

App::App(Context * context) : Application(context){}

void App::Setup(){
	if(!LedMatrixLibrary::Init()){
		Log(LOG_ERROR, "App", "LedMatrixLibrary initialization failed");
	}
	// engine params
	engineParameters_["FullScreen"] = false;
	engineParameters_["WindowWidth"] = DisplayManager::width;
	engineParameters_["WindowHeight"] = DisplayManager::height;
	engineParameters_["WindowResizable"] = false;
	std::string resPath = LEDLIB_RESOURCES_PATH;
	resPath += "/Urho3D";
	engineParameters_["ResourcePaths"] = "Data;CoreData";
	engineParameters_["ResourcePrefixPaths"] = resPath.c_str();
	engineParameters_["VSync"] = false;
	Log(LOG_INFO, "App", "ResourcePrefixPath=" + resPath);
	OnSetup();
}

void App::Start(){
	AppManager::Setup(this);
	Gfx::Setup();
	OnStart();
	GetSubsystem<Input>()->SetMouseGrabbed(false);
	SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(App, HandlePostRenderUpdate));
	SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(App, HandleUpdate));
	LedMatrixLibrary::Start();
}

void App::Stop(){
	OnStop();
	LedMatrixLibrary::Exit();
}

void App::HandleUpdate(StringHash eventType,VariantMap& eventData){
	LedMatrixLibrary::Update();
	if(LedMatrixLibrary::exitRequested){
		context_->GetSubsystem<Engine>()->Exit();
	} else {
		OnUpdate();
	}
}

void App::HandlePostRenderUpdate(StringHash eventType,VariantMap& eventData){
	Texture2D* texture = AppManager::GetRenderTexture();
	uint8_t* pixels = AppManager::GetPixelBuffer();
	texture->GetData(0, pixels);
	LedMatrixLibrary::Render();
}

}
