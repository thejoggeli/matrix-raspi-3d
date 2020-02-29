#include "Ledlib3dApplication.h"
#include "Urho3D/Core/CoreEvents.h"
#include "Ledlib/Ledlib.h"
#include "Ledlib/Display/DisplayManager.h"
#include "Ledlib/Log.h"
#include "Ledlib/Util/Strings.h"
#include "Urho3D/Graphics/Graphics.h"
#include "Urho3D/Graphics/Texture2D.h"
#include "Urho3D/Graphics/Camera.h"
#include "Urho3D/Graphics/Octree.h"
#include "Urho3D/Graphics/Viewport.h"
#include "Urho3D/Graphics/RenderSurface.h"
#include "Urho3D/Resource/Image.h"
#include "Urho3D/Input/Input.h"
#include "Urho3D/Scene/Scene.h"

using namespace Urho3D;

namespace Ledlib {

Ledlib3dApplication::Ledlib3dApplication(Context * context) : Application(context){

}

void Ledlib3dApplication::Setup(){
	if(!LedMatrixLibrary::Init()){
		Log(LOG_ERROR, "Ledlib3dApp", "LedMatrixLibrary initialization failed");
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
	Log(LOG_INFO, "Ledlib3dApp", "ResourcePrefixPath=" + resPath);
}

void Ledlib3dApplication::Start(){

	_scene = new Scene(GetContext());
	_scene->CreateComponent<Octree>();

	Context* context = GetContext();

	_renderTexture = new Texture2D(context);
	_renderTexture->SetSize(DisplayManager::width, DisplayManager::height, Graphics::GetRGBFormat(), Urho3D::TEXTURE_RENDERTARGET);
	_renderTexture->SetFilterMode(Urho3D::FILTER_DEFAULT);

	Node* cameraNode = _scene->CreateChild("Camera");
	_camera = cameraNode->CreateComponent<Camera>();

	SharedPtr<RenderSurface> renderSurface(_renderTexture->GetRenderSurface());
	SharedPtr<Viewport> viewport(new Viewport(context, _scene, _camera));
	renderSurface->SetViewport(0, viewport);
	renderSurface->SetUpdateMode(Urho3D::SURFACE_UPDATEALWAYS);

	uint32_t len_predict = sizeof(uint8_t) * DisplayManager::width * DisplayManager::height * 3;
	uint32_t len_actual = _renderTexture->GetDataSize(DisplayManager::width, DisplayManager::height);

	Log(LOG_DEBUG, "Ledlib3dApp", iLog << "Pixel buffer size predicted=" << len_predict << ", actual=" << len_actual);
	if(len_predict != len_actual){
		Log(LOG_ERROR, "Ledlib3dApp", iLog << "Pixel buffer size predicted length != actual length");
	}

	_pixels = (uint8_t*) malloc(len_actual);
	DisplayManager::SetPixelsPointer(static_cast<void*>(_pixels), 3);

	GetSubsystem<Input>()->SetMouseGrabbed(false);
	SubscribeToEvent(E_POSTRENDERUPDATE, URHO3D_HANDLER(Ledlib3dApplication, HandlePostRenderUpdate));
	SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Ledlib3dApplication, HandleUpdate));

	OnSetup();
	LedMatrixLibrary::Start();
	OnStart();

}

void Ledlib3dApplication::Stop(){
	OnExit();
	LedMatrixLibrary::Exit();
}

void Ledlib3dApplication::HandleUpdate(StringHash eventType,VariantMap& eventData){
	LedMatrixLibrary::Update();
	if(LedMatrixLibrary::exitRequested){
		context_->GetSubsystem<Engine>()->Exit();
	} else {
		OnUpdate();
	}
}

void Ledlib3dApplication::HandlePostRenderUpdate(StringHash eventType,VariantMap& eventData){
	Texture2D* texture = GetRenderTexture();
	uint8_t* pixels = GetPixelBuffer();
	texture->GetData(0, pixels);
	LedMatrixLibrary::Render();
}

SharedPtr<Scene> Ledlib3dApplication::GetScene(){
	return _scene;
}
SharedPtr<Texture2D> Ledlib3dApplication::GetRenderTexture(){
	return _renderTexture;
}
SharedPtr<Camera> Ledlib3dApplication::GetCamera(){
	return _camera;
}
uint8_t* Ledlib3dApplication::GetPixelBuffer(){
	return _pixels;
}

}
