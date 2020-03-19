#include <iostream>
#include <cmath>
#include <FreeImage.h>

#include "Ledlib2d/App.h"
#include "Ledlib/Util/Lodepng.h"
#include "Ledlib/Util/Base64Helper.h"
#include "Ledlib/Time.h"
#include "Ledlib/Log.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Events/EventManager.h"
#include "Ledlib/Events/Event.h"
#include "Ledlib2d/Resources/ResourceManager.h"
#include "Ledlib2d/Resources/Bitmap.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Transform.h"
#include "Ledlib2d/Gfx/Text.h"
#include "Ledlib2d/Gfx/Bitmaps.h"
#include "Ledlib2d/AppCamera.h"

using namespace std;
using namespace Ledlib;

static std::shared_ptr<Bitmap> canvas;
static std::shared_ptr<AppCamera> camera;

class WelcomeApp : public App {
	void OnSetup() override;
	void OnStart() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnExit() override;
	static void OnMessageClientConnected(void* obj, MessageEvent& event);
	static void OnMessageGalleryUploadPNG(void* obj, MessageEvent& event);
	static void OnMessageGalleryUploadJPG(void* obj, MessageEvent& event);
	static void CreateCanvasFromFIBITMAP(FIBITMAP* fibitmap, int w, int h);
};
void WelcomeApp::OnSetup() {
	camera = std::make_shared<AppCamera>();
	EventManager::SubscribeMessage("client_connected", this, &WelcomeApp::OnMessageClientConnected);
	EventManager::SubscribeMessage("gallery_upload_png", this, &WelcomeApp::OnMessageGalleryUploadPNG);
	EventManager::SubscribeMessage("gallery_upload_jpg", this, &WelcomeApp::OnMessageGalleryUploadJPG);
	ResourceManager::LoadFontBdf("font-1", "7x13B.bdf");
	ResourceManager::LoadFontBdf("font-2", "5x8.bdf");
	FreeImage_Initialise();
}
void WelcomeApp::OnStart(){

}
void WelcomeApp::OnUpdate() {
	camera->SimpleStartReset();
	camera->SimpleSelectReset();
	camera->SimpleArrowsZoom();
	camera->SimpleArrowsRotate();
	camera->SimpleJoystickMove();
}
void WelcomeApp::OnExit(){
	FreeImage_DeInitialise();
}
void WelcomeApp::OnRender() {
	/*
	BaseFont* font_1 = ResourceManager::GetFont("font-1");
	BaseFont* font_2 = ResourceManager::GetFont("font-2");

	Gfx::SetFont(font_1);
	Gfx::SetTextColor(1.0f, 1.0f, 0.0f);
	Gfx::DrawText("Gallery", 0, 5);

	Gfx::SetFont(font_2);
	Gfx::SetTextColor(1.0f, 1.0f, 1.0f);
	Gfx::DrawText("work in progress", sin(Time::sinceStart)*12, -5);
	*/

	if(canvas){
		Gfx::Save();
		camera->ApplyTransform();
		float scale = Gfx::height / static_cast<float>(canvas->height);
	//	scale *= ((sin(Time::sinceStart)*0.5f+0.5f)*4.0f+0.5f);
		Gfx::Scale(scale);
		Gfx::DrawBitmap(canvas.get(), 0.0f, 0.0f);
		Gfx::Restore();
	} else {
		BaseFont* font_1 = ResourceManager::GetFont("font-1");
		BaseFont* font_2 = ResourceManager::GetFont("font-2");

		Gfx::SetFont(font_1);
		Gfx::SetTextColor(1.0f, 1.0f, 0.0f);
		Gfx::DrawText("Gallery", 0, 5);

		Gfx::SetFont(font_2);
		Gfx::SetTextColor(1.0f, 1.0f, 1.0f);
		Gfx::DrawText("waiting for picture", sin(Time::sinceStart)*19.5f, -5);
	}

}
void WelcomeApp::OnMessageClientConnected(void *obj, MessageEvent &event){

}
void WelcomeApp::OnMessageGalleryUploadJPG(void *obj, MessageEvent &event){
	// base64 string to bytes
	const std::string& base64_string = event.GetParamConstRef(0);
	std::vector<unsigned char> bytes;
	Base64Helper::base64_decode(bytes, base64_string);
	// bytes to fibitmap
	FIMEMORY* stream = FreeImage_OpenMemory(bytes.data(), bytes.size());
	FIBITMAP* bitmap = FreeImage_LoadFromMemory(FREE_IMAGE_FORMAT::FIF_JPEG, stream);
	unsigned int w = FreeImage_GetWidth(bitmap);
	unsigned int h = FreeImage_GetHeight(bitmap);
	// fibitmap to ledlibbitmap
	Log(LOG_INFO, "Gallery", iLog << "UploadJPG w=" << w << ", h=" << h << " bytes=" << bytes.size());
	WelcomeApp::CreateCanvasFromFIBITMAP(bitmap, w, h);
}
void WelcomeApp::OnMessageGalleryUploadPNG(void *obj, MessageEvent &event){
	// base64 string to bytes
	const std::string& base64_string = event.GetParamConstRef(0);
	std::vector<unsigned char> bytes;
	Base64Helper::base64_decode(bytes, base64_string);
	// bytes to fibitmap
	FIMEMORY* stream = FreeImage_OpenMemory(bytes.data(), bytes.size());
	FIBITMAP* bitmap = FreeImage_LoadFromMemory(FREE_IMAGE_FORMAT::FIF_PNG, stream);
	unsigned int w = FreeImage_GetWidth(bitmap);
	unsigned int h = FreeImage_GetHeight(bitmap);
	Log(LOG_INFO, "Gallery", iLog << "UploadPNG w=" << w << ", h=" << h << " bytes=" << bytes.size());
	WelcomeApp::CreateCanvasFromFIBITMAP(bitmap, w, h);
}
void WelcomeApp::CreateCanvasFromFIBITMAP(FIBITMAP *fibitmap, int w, int h){
	RGBQUAD rgb;
	canvas = Bitmap::CreateEmpty(w, h);
	for(int x = 0; x < w; x++){
		for(int y = 0; y < h; y++){
			FreeImage_GetPixelColor(fibitmap, x, h-y, &rgb);
			int i = (y*w+x)*4;
			canvas->image[i+0] = rgb.rgbRed;
			canvas->image[i+1] = rgb.rgbGreen;
			canvas->image[i+2] = rgb.rgbBlue;
			canvas->image[i+3] = 0xFF;
		}
	}
	canvas->Update();
	canvas->SetFilteringTrilinear();
	canvas->GenerateMipmap();
}

int main(){
	App::Run(new WelcomeApp());
	return EXIT_SUCCESS;
}












