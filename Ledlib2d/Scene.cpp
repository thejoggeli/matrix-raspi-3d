#include "Scene.h"
#include "Entity.h"
#include "Camera.h"
#include "Ledlib/Log.h"
#include <algorithm>
#include "Physics/Collider.h"
#include "Physics/CollisionManager.h"


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
	for(auto& pair: _enabledCollisions){
		if(pair.first == pair.second){
			if(_colliders.find(pair.first) == _colliders.end()) continue;
			auto& v = _colliders[pair.first];
			for(int i = 0; i < v.size()-1; i++){
				for(int j = i+1; j < v.size(); j++){
					CollisionManager::CheckCollision(v[i], v[j]);
				}
			}
		} else {
			if(_colliders.find(pair.first) == _colliders.end()) continue;
			if(_colliders.find(pair.second) == _colliders.end()) continue;
			auto& va = _colliders[pair.first];
			auto& vb = _colliders[pair.second];
			for(int i = 0; i < va.size(); i++){
				for(int j = i; j < vb.size(); j++){
					CollisionManager::CheckCollision(va[i], vb[j]);
				}
			}
		}
	}
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
		int num = _destroyedEntities.size();
		for(int i = 0; i < num; i++){
			_destroyedEntities[i]->DestroyChildren(true);
		}
		for(auto& entity: _destroyedEntities){
			entity->SetParent(GetRoot());
			entity->OnEnd();
		}
		for(auto& entity: _destroyedEntities){
			_entities.erase(std::remove(_entities.begin(), _entities.end(), entity), _entities.end());
		}
		for(auto& entity: _destroyedEntities){
			if(entity->GetCollider()){
				UnregisterCollider(entity->GetCollider());
			}
		}
		for(auto& entity: _destroyedEntities){
			entity->SetParent(nullptr);
		}
		_destroyedEntities.clear();
	}
}

void Scene::SetDebugDrawEnabled(bool e){
	_debugDrawEnabled = e;
}

void Scene::EnableCollision(const std::string& a, const std::string& b){
	Log(LOG_DEBUG, "Scene", "Enabling collision: " + a + " <=> " + b);
	_enabledCollisions.push_back(std::make_pair(a, b));;
}
void Scene::RegisterCollider(const std::shared_ptr<Collider>& collider){
	_colliders[collider->group].push_back(collider.get());
}
void Scene::UnregisterCollider(const std::shared_ptr<Collider>& collider){
	auto search = _colliders.find(collider->group);
	if(search != _colliders.end()){
		auto& list = _colliders[collider->group];
		list.erase(std::remove(list.begin(), list.end(), collider.get()), list.end());
	} else {
		Log(LOG_ERROR, "Scene", "Collision group not found: " + collider->group);
	}
}

}
