#include <iostream>
#include <cmath>
#include <FreeImage.h>
#include <iomanip>

#include "Ledlib2d/App.h"
#include "Ledlib/Util/Lodepng.h"
#include "Ledlib/Util/Base64Helper.h"
#include "Ledlib/Time.h"
#include "Ledlib/Log.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Remote/ServerManager.h"
#include "Ledlib/Remote/ServerMessage.h"
#include "Ledlib/Events/EventManager.h"
#include "Ledlib/Events/Event.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Ledlib/Util/ColorHsl.h"
#include "Ledlib2d/Resources/ResourceManager.h"
#include "Ledlib2d/Resources/Bitmap.h"
#include "Ledlib2d/Resources/Font.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Gfx/Transform.h"
#include "Ledlib2d/Gfx/Text.h"
#include "Ledlib2d/Gfx/Bitmaps.h"
#include "Ledlib2d/AppCamera.h"

using namespace std;
using namespace Ledlib;

static std::shared_ptr<AppCamera> camera;

enum class MediaType {
    Image, Video
};

class Media {
public:
    int client = 0;
    MediaType mediaType;
    int numFramesExpected = 0;
    std::string format = "jpg";
    double fps = 30.0;
    double duration = 0.0;
    double frameDuration = 0.0;
    std::vector<std::shared_ptr<Bitmap>> frames;
    double currentTime = 0.0;
    float lastMessageTime = 0.0;
    bool reverse = false;
    bool playForward = true;
};

static std::shared_ptr<Media> activeMedia;
static std::shared_ptr<Media> streamingMedia;

class WelcomeApp : public App {
	void OnSetup() override;
	void OnStart() override;
	void OnUpdate() override;
	void OnRender() override;
	void OnExit() override;
	static void OnMessageClientConnected(void* obj, MessageEvent& event);
    static void OnMessageClientDisconnected(void* obj, MessageEvent& event);
    static void OnMessageGalleryUploadPNG(void* obj, MessageEvent& event);
    static void OnMessageGalleryUploadJPG(void* obj, MessageEvent& event);
    static void OnMessageUploadStreamBegin(void* obj, MessageEvent& event);
    static void OnMessageUploadStreamMetaData(void* obj, MessageEvent& event);
    static void OnMessageUploadStreamFrameData(void* obj, MessageEvent& event);
    static void OnMessageUploadStreamEnd(void* obj, MessageEvent& event);
    static std::shared_ptr<Bitmap> CreateCanvasFromFIBITMAP(FIBITMAP* fibitmap, int w, int h);
};
void WelcomeApp::OnSetup() {
	camera = std::make_shared<AppCamera>();
	EventManager::SubscribeMessage("client_connected", this, &WelcomeApp::OnMessageClientConnected);
    EventManager::SubscribeMessage("client_disconnected", this, &WelcomeApp::OnMessageClientDisconnected);
	EventManager::SubscribeMessage("gallery_upload_png", this, &WelcomeApp::OnMessageGalleryUploadPNG);
    EventManager::SubscribeMessage("gallery_upload_jpg", this, &WelcomeApp::OnMessageGalleryUploadJPG);
    EventManager::SubscribeMessage("upload_stream_begin", this, &WelcomeApp::OnMessageUploadStreamBegin);
    EventManager::SubscribeMessage("upload_stream_meta_data", this, &WelcomeApp::OnMessageUploadStreamMetaData);
    EventManager::SubscribeMessage("upload_stream_frame_data", this, &WelcomeApp::OnMessageUploadStreamFrameData);
    EventManager::SubscribeMessage("upload_stream_end", this, &WelcomeApp::OnMessageUploadStreamEnd);
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
    if(streamingMedia && Time::sinceStart - streamingMedia->lastMessageTime > 10.0){
        streamingMedia = nullptr;
    }
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

    if(activeMedia){
        if(activeMedia->mediaType == MediaType::Image){
            Gfx::Save();
            camera->ApplyTransform();
            Bitmap* canvas = activeMedia->frames[0].get();
            float scale = Gfx::height / static_cast<float>(canvas->height);
            Gfx::Scale(scale);
            Gfx::SetBitmapColor(1.0, 1.0, 1.0, 1.0);
            Gfx::DrawBitmap(canvas, 0.0f, 0.0f);
            Gfx::Restore();
        } else if(activeMedia->mediaType == MediaType::Video){
            Gfx::Save();
            camera->ApplyTransform();

            int frame_id, frame_id_2;
            float alpha;

            if(activeMedia->playForward){
                // current frame id
                frame_id = static_cast<int>(activeMedia->currentTime/activeMedia->frameDuration);
                if(frame_id >= activeMedia->frames.size()){
                    frame_id = 0;
                }
                // next frame id
                alpha = fmod(activeMedia->currentTime/activeMedia->frameDuration, 1.0);
                frame_id_2 = frame_id+1;
                if(frame_id_2 >= activeMedia->frames.size()){
                    if(activeMedia->reverse){
                        frame_id_2 = Numbers::Clamp(0, activeMedia->frames.size()-1, frame_id-1);
                    } else {
                        frame_id_2 = 0;
                    }
                }
            } else {
                // current frame id
                frame_id = static_cast<int>(activeMedia->currentTime/activeMedia->frameDuration);
                if(frame_id >= activeMedia->frames.size()){
                    frame_id = 0;
                }
                // prev frame id
                alpha = 1.0-fmod(activeMedia->currentTime/activeMedia->frameDuration, 1.0);
                frame_id_2 = frame_id-1;
                if(frame_id_2 < 0){
                    if(activeMedia->reverse){
                        frame_id_2 = Numbers::Clamp(0, activeMedia->frames.size()-1, frame_id+1);
                    } else {
                        frame_id_2 = activeMedia->frames.size()-1;
                    }
                }
            }

            Bitmap* canvas = activeMedia->frames[frame_id].get();
            Bitmap* canvas_2 = activeMedia->frames[frame_id_2].get();

            float scale = Gfx::height / static_cast<float>(canvas->height);
            Gfx::Scale(scale);

            Gfx::SetBitmapColor(1.0, 1.0, 1.0, 1.0);
            Gfx::DrawBitmap(canvas, 0.0f, 0.0f);

            Gfx::SetBitmapColor(1.0, 1.0, 1.0, alpha);
            Gfx::DrawBitmap(canvas_2, 0.0f, 0.0f);

            Gfx::Restore();
            if(activeMedia->playForward){
                activeMedia->currentTime += static_cast<double>(Time::deltaTime);
                if(activeMedia->currentTime > activeMedia->duration){
                    double overtime = fmod(activeMedia->currentTime, activeMedia->duration);
                    if(activeMedia->reverse){
                        activeMedia->currentTime = activeMedia->duration - overtime - activeMedia->frameDuration;
                        activeMedia->playForward = false;
                    } else {
                        activeMedia->currentTime = overtime;
                    }
                }
            } else {
                activeMedia->currentTime -= static_cast<double>(Time::deltaTime);
                if(activeMedia->currentTime < 0){
                    double undertime = fmod(abs(activeMedia->currentTime), activeMedia->duration);
                    if(activeMedia->reverse){
                        activeMedia->currentTime = undertime + activeMedia->frameDuration;
                        activeMedia->playForward = true;
                    } else {
                        activeMedia->currentTime = activeMedia->duration - undertime;
                    }
                }
            }
        }
	} else {
        Gfx::antiKerning = true;
        Gfx::SetTextPosition(TextAlign::Center, TextBaseline::Middle);
		BaseFont* font_1 = ResourceManager::GetFont("font-1");
		BaseFont* font_2 = ResourceManager::GetFont("font-2");

		Gfx::SetFont(font_1);
		Gfx::SetTextColor(1.0f, 1.0f, 0.0f);
		Gfx::DrawText("Gallery", 0, 5);

		Gfx::SetFont(font_2);
		Gfx::SetTextColor(1.0f, 1.0f, 1.0f);
        Gfx::DrawText("waiting for image", sin(Time::sinceStart)*19.5f, -5);
    }
    if(streamingMedia){
        Gfx::Save();
        double progress = (double)streamingMedia->frames.size() / (double)streamingMedia->numFramesExpected;
        stringstream ss;
        ss << std::fixed << std::setprecision(0) << (progress*100.0) << "%";
        std::string progress_str = ss.str();

        Gfx::antiKerning = true;
        Gfx::SetTextPosition(TextAlign::Center, TextBaseline::Middle);
        BaseFont* font_1 = ResourceManager::GetFont("font-1");

        ColorRgb borderColor = ColorHsl::HslToRgb(progress/3.0, 1.0, 0.5);
        ColorRgb bgColor = ColorRgb::BLACK;

        Gfx::SetDrawColor(borderColor);
        Gfx::FillRect(0, 0, 32+2, 16+2);

        Gfx::SetDrawColor(bgColor);
        Gfx::FillRect(0, 0, 32, 16);

        Gfx::SetFont(font_1);
        Gfx::SetTextColor(borderColor);
        Gfx::DrawText(progress_str, 0, 0);

        Gfx::Restore();
    }

}
void WelcomeApp::OnMessageClientConnected(void *obj, MessageEvent &event){

}
void WelcomeApp::OnMessageClientDisconnected(void *obj, MessageEvent &event){
    if(streamingMedia && event.clientId == streamingMedia->client){
        streamingMedia = nullptr;
    }
}

void WelcomeApp::OnMessageUploadStreamBegin(void *obj, MessageEvent &event){
    Log(LOG_INFO, "Gallery", iLog << "UploadStreamBegin, client = " << event.clientId);
    streamingMedia = std::make_shared<Media>();
    streamingMedia->client = event.clientId;
    streamingMedia->lastMessageTime = Time::sinceStart;
}
void WelcomeApp::OnMessageUploadStreamMetaData(void *obj, MessageEvent &event){
    Log(LOG_INFO, "Gallery", iLog << "UploadStreamMetaData, client = " << event.clientId);
    if(!streamingMedia || streamingMedia->client != event.clientId){
        Log(LOG_INFO, "Gallery", "UploadStreamMetaData denied, another stream is already running");
        return;
    }
    std::string mediaType = event.GetParam(0);
    Log(LOG_INFO, "Gallery", iLog << "UploadStreamMetaData mediaType = " << mediaType);
    streamingMedia->lastMessageTime = Time::sinceStart;
    if(mediaType == "image"){
        streamingMedia->mediaType = MediaType::Image;
        streamingMedia->format = event.GetParam(1);
        streamingMedia->fps = 0.0;
        streamingMedia->frameDuration = 0.0;
        streamingMedia->numFramesExpected = event.GetParamInt(3);
        streamingMedia->reverse = false;
    } else if(mediaType == "video"){
        streamingMedia->mediaType = MediaType::Video;
        streamingMedia->format = event.GetParam(1);
        streamingMedia->fps = event.GetParamDouble(2);
        streamingMedia->frameDuration = 1.0/streamingMedia->fps;
        streamingMedia->numFramesExpected = event.GetParamInt(3);
        streamingMedia->reverse = event.GetParamBool(4);
    } else {
        Log(LOG_ERROR, "Gallery", iLog << "unkown media type: " << mediaType);
    }
}
void WelcomeApp::OnMessageUploadStreamFrameData(void *obj, MessageEvent &event){
    Log(LOG_INFO, "Gallery", iLog << "UploadStreamFrameData, client = " << event.clientId);
    if(!streamingMedia || streamingMedia->client != event.clientId){
        Log(LOG_INFO, "Gallery", "UploadStreamFrameData denied, another stream is already running");
        return;
    }
    streamingMedia->lastMessageTime = Time::sinceStart;
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
    // add bitmap to frames
    streamingMedia->frames.push_back(WelcomeApp::CreateCanvasFromFIBITMAP(bitmap, w, h));
    // respond
    ServerMessage msg = ServerMessage("frame-received");
    ServerManager::SendMessage(msg, event.clientId);
}
void WelcomeApp::OnMessageUploadStreamEnd(void *obj, MessageEvent &event){
    Log(LOG_INFO, "Gallery", iLog << "UploadStreamEnd, client = " << event.clientId);
    if(!streamingMedia || streamingMedia->client != event.clientId){
        Log(LOG_INFO, "Gallery", "UploadStreamEnd denied, another stream is already running");
        return;
    }
    Log(LOG_INFO, "Gallery", iLog << "UploadStreamEnd " << streamingMedia->frames.size() << "/" << streamingMedia->numFramesExpected << " frames received");
    streamingMedia->lastMessageTime = Time::sinceStart;
    if(streamingMedia->frames.size() < 1){
        Log(LOG_ERROR, "Gallery", "Invalid media received, frames = 0");
    } else if(streamingMedia->frames.size() == 1){
        streamingMedia->mediaType = MediaType::Image;
        activeMedia = streamingMedia;
    } else {
        streamingMedia->duration = streamingMedia->frameDuration * streamingMedia->frames.size();
        activeMedia = streamingMedia;
    }
    streamingMedia = nullptr;
}

void WelcomeApp::OnMessageGalleryUploadJPG(void *obj, MessageEvent &event){
    /*
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
    canvas = WelcomeApp::CreateCanvasFromFIBITMAP(bitmap, w, h);
    */
}
void WelcomeApp::OnMessageGalleryUploadPNG(void *obj, MessageEvent &event){
    /*
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
    canvas = WelcomeApp::CreateCanvasFromFIBITMAP(bitmap, w, h);
    */
}
std::shared_ptr<Bitmap> WelcomeApp::CreateCanvasFromFIBITMAP(FIBITMAP *fibitmap, int w, int h){
	RGBQUAD rgb;
    std::shared_ptr<Bitmap> canvas = Bitmap::CreateEmpty(w, h);
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
    return canvas;
}

int main(){
	App::Run(new WelcomeApp());
	return EXIT_SUCCESS;
}












