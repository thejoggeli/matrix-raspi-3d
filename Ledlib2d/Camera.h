#pragma once

#include <glm/mat4x4.hpp>
#include <memory>

namespace Ledlib {

class Entity;

class Camera {
private:
	std::weak_ptr<Entity> entity;
public:
	glm::mat4 projectionMatrix;
	Camera();
	static std::shared_ptr<Camera> Create();
	std::shared_ptr<Entity> GetEntity();
	void SetEntity(const std::shared_ptr<Entity>& entity);
	void SimpleJoystickMove(float sensitivity = 20.0f);
	void SimpleJoystickRotate(float sensitivity = 1.0f);
	void SimpleJoystickZoom(float sensitivity = 1.0f);
	void SimpleArrowsMove(float sensitivity = 20.0f);
	void SimpleArrowsRotate(float sensitivity = 1.0f);
	void SimpleArrowsZoom(float sensitivity = 1.0f);
	void ApplyTransform();
	void SetPerspective(float fovy);
	void SetPerspective(float fovy, float aspect, float near, float far);
	void SetOrthographic();
	void SetOrthographic(float left, float right, float bottom, float top, float near, float far);

};

}

