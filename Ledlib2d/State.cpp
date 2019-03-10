#include "State.h"
#include "Game.h"
#include "Ledlib/Log.h"
#include "Scene/Camera.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"

namespace Ledlib {

int State::idCounter = 0;

State::State(){
	_id = idCounter++;
	Log(LOG_DEBUG, "State", iLog << "State created (id=" << _id << ")");
}
State::~State(){
	Log(LOG_DEBUG, "State", iLog << "State destroyed (id=" << _id << ")");
}
std::shared_ptr<State> State::Init(const std::shared_ptr<State>& state, const std::shared_ptr<Game>& game){
	state->_game = game;
	return state;
}
std::shared_ptr<Scene> State::GetScene(){
	return _scene;
}
std::shared_ptr<Camera> State::GetCamera(){
	if(auto p = _cameraEntity.lock()){
		return p->GetCamera();
	}
	Log(LOG_ERROR, "State", "Camera expired");
	return nullptr;
}
std::shared_ptr<Game> State::GetGame(){
	if(auto p = _game.lock()){
		return p;
	}
	Log(LOG_ERROR, "State", "Game expired");
	return nullptr;
}

void State::Start(){
	// scene
	_scene = Scene::Create();
	// camera
	std::shared_ptr<Entity> cameraEntity = _scene->CreateEntity<Entity>();
	cameraEntity->CreateCamera();
	_cameraEntity = cameraEntity;
}

void State::Update(){
	_scene->Update();
}

}
