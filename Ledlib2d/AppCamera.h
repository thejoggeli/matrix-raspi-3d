#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Ledlib {

class Entity;

class AppCamera {
public:

	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	glm::quat rotation = glm::quat(1, 0, 0, 0);
	glm::mat4 transformMatrix = glm::mat4(1.0);
	glm::mat4 projectionMatrix;

	AppCamera();

	void UpdateTransformMatrix();

	void SetPosition(float x, float y, float z = 0);
	void SetPosition(const glm::vec2& v);
	void SetPosition(const glm::vec3& v);
	void Translate(float x, float y, float z = 0);
	void Translate(const glm::vec2& v);
	void Translate(const glm::vec3& v);
	void Move(float x, float y, float z = 0);
	void Move(const glm::vec2& v);
	void Move(const glm::vec3& v);

	void SetScale(float s);
	void SetScale(float x, float y, float z = 1);
	void SetScale(const glm::vec3& v);
	void Scale(float s);
	void Scale(float x, float y, float z = 1);
	void Scale(const glm::vec3& v);

	void SetRotation(float z);
	void SetRotation(const glm::mat4& rotation);
	void SetRotation(const glm::quat& rotation);
	void SetRotation(const glm::vec3 eulers);
	void Rotate(float z);
	void Rotate(const glm::quat& rotation);
	float GetAngle();
	glm::vec3 GetEulers();

	void Reset();

	void SimpleStartReset();
	void SimpleSelectReset();
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

