#include "AppManager.h"
#include "App.h"
#include "Urho3D/Scene/Scene.h"
#include "Urho3D/Graphics/Graphics.h"
#include "Urho3D/Graphics/Octree.h"
#include "Urho3D/Graphics/Texture2D.h"
#include "Urho3D/Graphics/Viewport.h"
#include "Urho3D/Graphics/RenderSurface.h"
#include "Urho3D/Graphics/Camera.h"
#include "Ledlib/Display/DisplayManager.h"
#include "Ledlib/Log.h"
#include <stdlib.h>

using namespace Urho3D;
using namespace std;

namespace Ledlib {

static SharedPtr<App> _app;
static SharedPtr<Scene> _scene;
static SharedPtr<Camera> _camera;
static SharedPtr<Texture2D> _renderTexture;
static uint8_t* _pixels;

AppManager::AppManager(){}

void AppManager::Setup(App* app){

	_app = app;
	_scene = new Scene(app->GetContext());
	_scene->CreateComponent<Octree>();

	Context* context = app->GetContext();

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

	Log(LOG_DEBUG, "AppManager", iLog << "Pixel buffer size predicted=" << len_predict << ", actual=" << len_actual);
	if(len_predict != len_actual){
		Log(LOG_ERROR, "AppManager", iLog << "Pixel buffer size predicted length != actual length");
	}

	_pixels = (uint8_t*) malloc(len_actual);
	DisplayManager::SetPixelsPointer(static_cast<void*>(_pixels), 3);

}

SharedPtr<App> AppManager::GetApp(){
	return _app;
}
SharedPtr<Scene> AppManager::GetScene(){
	return _scene;
}
SharedPtr<Texture2D> AppManager::GetRenderTexture(){
	return _renderTexture;
}
SharedPtr<Camera> AppManager::GetCamera(){
	return _camera;
}
uint8_t* AppManager::GetPixelBuffer(){
	return _pixels;
}

}
