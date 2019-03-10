#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <glm/vec3.hpp>
#include "Entity.h"

namespace Ledlib {

class Entity;
class Collider;

class Scene : public std::enable_shared_from_this<Scene> {
private:
	static int idCounter;
	int _id;
	std::shared_ptr<Entity> _root;
	std::vector<Entity*> _entities;
	std::vector<Entity*> _addedEntities;
	std::vector<Entity*> _destroyedEntities;
	std::unordered_map<std::string, std::vector<Collider*>> _colliders;
	std::vector<std::pair<std::string, std::string>> _enabledCollisions;
	bool _debugDrawEnabled = false;
	std::weak_ptr<Entity> camera;
public:
	const bool& debugDrawEnabled = _debugDrawEnabled;

	Scene();
	virtual ~Scene();

	static std::shared_ptr<Scene> Create();

	template<typename T, typename std::enable_if<std::is_base_of<Entity, T>::value>::type* = nullptr>
	std::shared_ptr<T> CreateEntity(const std::string& name, const std::shared_ptr<Entity>& parent = nullptr){
		return Entity::Create<T>(shared_from_this(), name, parent);
	}
	void OnEntityCreated(const std::shared_ptr<Entity>& entity);
	void OnEntityDestroyed(const std::shared_ptr<Entity>& entity);
	std::vector<Entity*>& GetEntities();

	void EnableCollision(const std::string& group_a, const std::string& group_b);
	void RegisterCollider(const std::shared_ptr<Collider>& collider);
	void UnregisterCollider(const std::shared_ptr<Collider>& collider);

	std::shared_ptr<Entity> GetRoot();

	void Update();
	void SetDebugDrawEnabled(bool e);

};

}
