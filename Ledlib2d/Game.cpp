#include "Game.h"
#include "State.h"
#include "Entity.h"
#include "Gfx/Gfx.h"
#include "Gfx/Renderer.h"
#include "Resources/ResourceManager.h"
#include "Ledlib/Ledlib.h"
#include "Ledlib/Log.h"
#include "Ledlib/Display/DisplayManager.h"
#include <stdlib.h>
#include "Ledlib/Util/Timer.h"
#include "Physics/Collider.h"

namespace Ledlib {

int Game::idCounter = 0;

std::shared_ptr<Game> Game::instance = nullptr;

Game::Game(){
	_id = idCounter++;
	Log(LOG_DEBUG, "Game", iLog << "Game created (id=" << _id << ")");
}
Game::~Game(){
	Log(LOG_DEBUG, "Game", iLog << "Game destroyed (id=" << _id << ")");
}

void Game::Run(Game* game){
	if(instance){
		Log(LOG_ERROR, "Game", "Game already initialized");
		return;
	} else {
		instance = std::shared_ptr<Game>(game);
		instance->Loop();
		instance = nullptr;
	}
}

void Game::Loop(){

	// init
	LedMatrixLibrary::Init();
	ResourceManager::Init();
	Gfx::Init();

	// setup
	OnSetup();

	// start
	LedMatrixLibrary::Start();
	OnStart();
	activeState->Start();
	activeState->OnStart();
	if(!activeState){
		Log(LOG_ERROR, "Game", "No state set");
		return;
	}

	// game loop
	Timer timer;
	timer.Start(10.0f);
	while(!LedMatrixLibrary::exitRequested){
		LedMatrixLibrary::Update();
		activeState->Update();
		activeState->OnUpdate();
		activeState->OnLateUpdate();
		Gfx::RenderPrepare();
		activeState->OnRender();
		Gfx::Render(activeState->GetScene(), activeState->GetCamera());
		activeState->OnAfterRender();
		Gfx::UpdatePixelBuffer();
		LedMatrixLibrary::Render();
		if(queuedState){
			activeState->OnExit();
			activeState = queuedState;
			queuedState = nullptr;
			activeState->Start();
			activeState->OnStart();
		}
		if(timer.IsFinished()){
			timer.Restart(true);
			Log(LOG_DEBUG, "Game", iLog
				<< "Local=" << Entity::localUpdateCounter
				<< " / World=" << Entity::worldUpdateCounter
				<< " / Collider " << Collider::updateCounter
			);
			Entity::localUpdateCounter = Entity::worldUpdateCounter = Collider::updateCounter = 0;
		}
	}

	// exit
	activeState->OnExit();
	OnExit();
	LedMatrixLibrary::Exit();
}

std::shared_ptr<Game> Game::GetInstance(){
	return instance;
}

Camera* Game::GetCameraFromState(){
	return instance->activeState->GetCamera();
}

Entity* Game::GetCameraEntityFromState(){
	return instance->activeState->GetCameraEntity();
}

Scene* Game::GetSceneFromState(){
	return instance->activeState->GetScene();
}

std::shared_ptr<State> Game::SetState(const std::shared_ptr<State>& state){
	state->SetGame(this);
	if(activeState == nullptr){
		activeState = state;
	} else {
		queuedState = state;
	}
	return state;
}
std::shared_ptr<State> Game::GetState(){
	return activeState;
}

}
