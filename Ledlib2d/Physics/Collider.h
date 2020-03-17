#pragma once

#include <memory>
#include <glm/vec2.hpp>

namespace Ledlib {

class Entity;

enum class ColliderType {
	None, Point, Circle, Box, Polygon
};

class Collider {
private:
	bool _needsUpdateFlag = false;
	std::weak_ptr<Entity> _entity;
	std::string _group;
public:
	bool drawBoundingCircle;
	bool drawCenter;
	const std::string& group = _group;
	static int updateCounter;
	const bool& needsUpdate = _needsUpdateFlag;
	glm::vec2 _position;
	float _rotation;
	glm::vec2 _scale;
	float _boundingRadius, _boundingRadiusSquared;

	ColliderType type = ColliderType::None;
	Collider();
	virtual ~Collider();

	template<typename T, typename std::enable_if<std::is_base_of<Collider, T>::value>::type* = nullptr>
	static std::shared_ptr<T> Create(const std::string& group){
		std::shared_ptr<T> collider = std::make_shared<T>();
		collider->Setup(group);
		return collider;
	}

	void SetNeedsUpdate();

	void SetEntity(const std::shared_ptr<Entity>& entity);
	std::shared_ptr<Entity> GetEntity();

	void Setup(const std::string& group);
	virtual void OnSetup(){}

	void UpdateTransform();
	virtual void OnUpdateTransform(){}

	void DebugDraw();
	virtual void OnDebugDraw(){}

};

}


