#pragma once

#include <vector>
#include <memory>
#include <glm/vec3.hpp>
#include "Entity.h"

namespace Ledlib {

class Entity;

class Scene : public std::enable_shared_from_this<Scene> {
private:
	static int idCounter;
	int _id;
	std::shared_ptr<Entity> _root;
	std::vector<Entity*> _entities;
public:
	Scene();
	virtual ~Scene();

	static std::shared_ptr<Scene> Create();

	template<typename T, typename std::enable_if<std::is_base_of<Entity, T>::value>::type* = nullptr>
	std::shared_ptr<T> CreateEntity(const std::shared_ptr<Entity>& parent = nullptr){
		return Entity::Create<T>(shared_from_this(), parent);
	}
	void RegisterEntity(const std::shared_ptr<Entity>& entity);
	void UnregisterEntity(const std::shared_ptr<Entity>& entity);
	std::vector<Entity*>& GetEntities();

	std::shared_ptr<Entity> GetRoot();

	void Update();

};

}
