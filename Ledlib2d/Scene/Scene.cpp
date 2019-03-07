#include "Scene.h"
#include "Entity.h"
#include "Camera.h"
#include "Ledlib/Log.h"
#include <algorithm>


using namespace std;

namespace Ledlib {

int Scene::idCounter = 0;

Scene::Scene(){
	_id = idCounter++;
	Log(LOG_DEBUG, "Scene", iLog << "Scene created (id=" << _id << ")");
}
Scene::~Scene(){
	Log(LOG_DEBUG, "Scene", iLog << "Scene destroyed (id=" << _id << ")");
}

std::shared_ptr<Scene> Scene::Create(){
	std::shared_ptr<Scene> scene = std::make_shared<Scene>();
	scene->_root = Entity::CreateRoot(scene);
	return scene;
}

void Scene::RegisterEntity(const std::shared_ptr<Entity>& entity){
	_entities.push_back(entity.get());
	entity->OnStart();
}

void Scene::UnregisterEntity(const std::shared_ptr<Entity>& entity){
	entity->OnEnd();
	_entities.erase(std::remove(_entities.begin(), _entities.end(), entity.get()), _entities.end());
}

std::vector<Entity*>& Scene::GetEntities(){
	return _entities;
}

std::shared_ptr<Entity> Scene::GetRoot(){
	return _root;
}

void Scene::Update(){
	for(auto& entity: _entities){
		entity->OnUpdate();
	}
	for(auto& entity: _entities){
		entity->OnLateUpdate();
	}
}

}
