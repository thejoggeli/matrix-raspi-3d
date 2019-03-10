#pragma once

#include <memory>

namespace Ledlib {

class Entity;

class Camera {
private:
	std::weak_ptr<Entity> entity;
public:
	Camera();
	static std::shared_ptr<Camera> Create();
	std::shared_ptr<Entity> GetEntity();
	void SetEntity(const std::shared_ptr<Entity>& entity);
	void SimpleJoystickMove();
	void SimpleJoystickRotate();
	void SimpleJoystickZoom();
	void SimpleArrowsMove();
	void SimpleArrowsRotate();
	void SimpleArrowsZoom();
	void ApplyTransform();
};

}

