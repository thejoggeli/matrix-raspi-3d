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

void Scene::OnEntityCreated(const std::shared_ptr<Entity> &entity){
	_addedEntities.push_back(entity.get());
}

void Scene::OnEntityDestroyed(const std::shared_ptr<Entity>& entity){
	if(std::find(_destroyedEntities.begin(), _destroyedEntities.end(), entity.get()) == _destroyedEntities.end()) {
		_destroyedEntities.push_back(entity.get());
	}
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
	if(_addedEntities.size() > 0){
		for(auto& entity: _addedEntities){
			_entities.push_back(entity);
		}
		for(auto& entity: _addedEntities){
			entity->OnStart();
		}
		_addedEntities.clear();
	}
	if(_destroyedEntities.size() > 0){
		for(auto& entity: _destroyedEntities){
			entity->OnEnd();
		}
		for(auto& entity: _destroyedEntities){
			while(entity->GetChildren().size() > 0){
				auto& child = entity->GetChildren()[0];
				child->SetPosition(child->GetWorldPosition());
				child->SetRotation(child->GetWorldRotation());
				child->SetScale(child->GetWorldScale());
				child->SetParent(_root);
			}
		}
		for(auto& entity: _destroyedEntities){
			_entities.erase(std::remove(_entities.begin(), _entities.end(), entity), _entities.end());
		}
		for(auto& entity: _destroyedEntities){
			entity->SetParent(nullptr);
		}
		_destroyedEntities.clear();
	}
}

}
