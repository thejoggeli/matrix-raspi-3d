#pragma once

#include <memory>

namespace Ledlib {

class Entity;

class Camera {
private:
	std::weak_ptr<Entity> entity;
public:
	Camera();
	static std::shared_ptr<Camera> Create(const std::shared_ptr<Entity>& entity);
	std::shared_ptr<Entity> GetEntity();
	void SimpleMove();
	void SimpleRotate();
	void SimpleZoom();
};

}

