#include "App.h"
#include "Gfx/Gfx.h"
#include "Gfx/Renderer.h"
#include "Resources/ResourceManager.h"
#include "Ledlib/Ledlib.h"
#include "Ledlib/Log.h"
#include "Ledlib/Display/DisplayManager.h"

namespace Ledlib {

int App::idCounter = 0;

std::shared_ptr<App> App::instance = nullptr;

App::App(){
	_id = idCounter++;
	Log(LOG_DEBUG, "App", iLog << "App created (id=" << _id << ")");
}
App::~App(){
	Log(LOG_DEBUG, "App", iLog << "App destroyed (id=" << _id << ")");
}

void App::Run(App* app){
	if(instance){
		Log(LOG_ERROR, "App", "App already initialized");
		return;
	} else {
		instance = std::shared_ptr<App>(app);
		instance->Loop();
		instance = nullptr;
	}
}

void App::Loop(){

	// init
	LedMatrixLibrary::Init();
	ResourceManager::Init();
	Gfx::Init();

	// setup
	OnSetup();

	// start
	LedMatrixLibrary::Start();
	OnStart();

	// app loop
	while(!LedMatrixLibrary::exitRequested){
		LedMatrixLibrary::Update();
		Gfx::RenderPrepare();
		OnRender();
		Gfx::UpdatePixelBuffer();
		LedMatrixLibrary::Render();
	}

	// exit
	OnExit();
	LedMatrixLibrary::Exit();
}

std::shared_ptr<App> App::GetInstance(){
	return instance;
}

}
