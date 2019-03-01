#include "Urho3DApplication.h"
#include "Urho3D/Core/CoreEvents.h"
#include "Ledlib/Ledlib.h"
#include "Ledlib/Display/DisplayManager.h"
#include "Ledlib/Log.h"
#include "Ledlib/Util/Strings.h"
#include "Urho3D/Graphics/Graphics.h"

using namespace Urho3D;

namespace Ledlib {

Urho3DApplication::Urho3DApplication(Context * context) : Application(context){

}

void Urho3DApplication::Setup(){
	if(!LedMatrixLibrary::Init()){
		Log(LOG_ERROR, "GlueApplication", "LedMatrixLibrary initialization failed");
	}
	// display setup
	width = DisplayManager::width;
	height = DisplayManager::height;
	pitch = 4;
//	pixels.assign(DisplayManager::width * DisplayManager::height * pitch, 255);
	DisplayManager::SetPixelsPointer(static_cast<void*>(pixels), pitch);
	// pixels image
	pixelsImage = new Image(context_);
	pixelsImage->SetSize(width, height, 4);
	// engine params
	engineParameters_["FullScreen"] = false;
	engineParameters_["WindowWidth"] = width;
	engineParameters_["WindowHeight"] = height;
	engineParameters_["WindowResizable"] = false;
	std::string resPath = LEDLIB_RESOURCES_PATH;
	resPath += "/Urho3D";
	engineParameters_["ResourcePaths"] = "Data;CoreData";
	engineParameters_["ResourcePrefixPaths"] = resPath.c_str();
	engineParameters_["VSync"] = false;
	Log(LOG_INFO, "Urho3DApplication", "ResourcePrefixPath=" + resPath);
}

void Urho3DApplication::Start(){
//	GetSubsystem<Input>()->SetMouseGrabbed(false);
	SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(Urho3DApplication, HandlePostRenderUpdate));
	SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Urho3DApplication, HandleUpdate));
	SubscribeToEvent(E_ENDFRAME, URHO3D_HANDLER(Urho3DApplication, HandleEndFrame));
	LedMatrixLibrary::Start();
}

void Urho3DApplication::Stop(){
	LedMatrixLibrary::Exit();
}

void Urho3DApplication::HandleUpdate(StringHash eventType,VariantMap& eventData){
	LedMatrixLibrary::Update();
}

void Urho3DApplication::HandlePostRenderUpdate(StringHash eventType,VariantMap& eventData){}

void Urho3DApplication::HandleEndFrame(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData){
	GetSubsystem<Graphics>()->TakeScreenShot(*pixelsImage);
	int i = 0;
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			pixels[i++] = pixelsImage->GetPixelInt(x,y);
		}
	}
	LedMatrixLibrary::Render();
}

}
