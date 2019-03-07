#pragma once

#include <memory>

namespace Ledlib {

class Entity;

class Camera {
	std::weak_ptr<Entity> entity;
public:
	Camera();
	static std::shared_ptr<Camera> Create();
	void SetEntity(std::shared_ptr<Entity> entity);
	std::shared_ptr<Entity> GetEntity();


};

}

