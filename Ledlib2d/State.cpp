#include "State.h"
#include "Game.h"
#include "Ledlib/Log.h"
#include "Camera.h"
#include "Scene.h"
#include "Entity.h"

namespace Ledlib {

int State::idCounter = 0;

State::State(){
	_id = idCounter++;
	Log(LOG_DEBUG, "State", iLog << "State created (id=" << _id << ")");
}
State::~State(){
	Log(LOG_DEBUG, "State", iLog << "State destroyed (id=" << _id << ")");
}
Scene* State::GetScene(){
	return _scene.get();
}
Camera* State::GetCamera(){
	if(auto p = _camera.lock()){
		return p.get();
	} else {
		Entity* cameraEntity = _scene->CreateEntity<Entity>("camera").get();
		_camera = cameraEntity->CreateCamera();
		return _camera.lock().get();
	}
}
Entity* State::GetCameraEntity(){
	return GetCamera()->GetEntity();
}
Game* State::GetGame(){
	return _game;
}
void State::SetGame(Game* game){
	_game = game;
}

void State::Start(){
	// scene
	_scene = Scene::Create();
	// camera
	Entity* cameraEntity = _scene->CreateEntity<Entity>("camera").get();
	_camera = cameraEntity->CreateCamera();
}

void State::Update(){
	_scene->Update();
}

}
