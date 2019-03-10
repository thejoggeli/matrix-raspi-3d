#pragma once

#include <memory>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Ledlib {

class Scene;
class Camera;
class Collider;
class CollisionData;

class Entity : public std::enable_shared_from_this<Entity> {
private:	
	static int idCounter;
	static int aliveCounter;
	int _id;

	bool _destroyed = false;
	std::weak_ptr<Entity> _parent;
	std::vector<std::shared_ptr<Entity>> _children;
	glm::vec3 _position = glm::vec3(0,0,0);
	glm::vec3 _scale = glm::vec3(1,1,1);
	glm::quat _rotation = glm::quat(1, 0, 0, 0);
	glm::mat4 _matrix = glm::mat4(1.0);
	glm::mat4 _worldMatrix = glm::mat4(1.0);
	glm::quat _worldRotation;
	bool _needsLocalUpdate = false;
	bool _needsWorldUpdate = false;
	std::weak_ptr<Scene> _scene;
	std::shared_ptr<Camera> _camera = nullptr;
	std::shared_ptr<Collider> _collider = nullptr;

public:
	std::string name = "unnamed";
	std::vector<std::string> tags;

	static int worldUpdateCounter;
	static int localUpdateCounter;
	Entity();
	virtual ~Entity();

	const bool &destroyed = _destroyed;
	const glm::vec3& position = _position;
	const glm::vec3& scale = _scale;
	const glm::quat& rotation = _rotation;
	const int& id = _id;

	template<typename T, typename std::enable_if<std::is_base_of<Entity, T>::value>::type* = nullptr>
	static std::shared_ptr<T> Create(const std::shared_ptr<Scene>& scene, const std::string& name, const std::shared_ptr<Entity>& parent){
		return std::static_pointer_cast<T>(Init(std::make_shared<T>(), scene, name, parent));
	}
	static std::shared_ptr<Entity> Init(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Scene>& scene, const std::string& name, const std::shared_ptr<Entity>& parent);
	static std::shared_ptr<Entity> CreateRoot(const std::shared_ptr<Scene>& scene);

	std::shared_ptr<Scene> GetScene();

	void SetCamera(const std::shared_ptr<Camera>& camera);
	std::shared_ptr<Camera> GetCamera();

	void SetCollider(const std::shared_ptr<Collider>& collider);
	std::shared_ptr<Collider> GetCollider();
	template<typename T, typename std::enable_if<std::is_base_of<Collider, T>::value>::type* = nullptr>
	std::shared_ptr<T> GetCollider(){
		return std::static_pointer_cast<T>(_collider);
	}

	void AddTag(const std::string& tag);
	void RemoveTag(const std::string& tag);
	bool HasTag(const std::string& tag);

	void SetPosition(float x, float y, float z = 0);
	void SetPosition(const glm::vec3& v);
	void Translate(float x, float y, float z = 0);
	void Translate(const glm::vec3& v);
	void Move();
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
	void Rotate(float z);
	void Rotate(const glm::quat& rotation);
	float GetAngle();
	float GetWorldAngle();

	void AddChild(std::shared_ptr<Entity> child);
	void SetParent(std::shared_ptr<Entity> parent);
	std::vector<std::shared_ptr<Entity>>& GetChildren();
	std::shared_ptr<Entity> GetParent();

	void SetNeedsWorldUpdate();
	void SetNeedsLocalUpdate();

	void Destroy();
	void DestroyChildren(bool recursive);

	glm::mat4& GetWorldMatrix();
	glm::mat4& GetMatrix();
	glm::quat& GetWorldRotation();
	glm::vec3 GetWorldPosition();
	glm::vec3 GetWorldScale();
	void UpdateMatrix();
	void UpdateWorldMatrix();

	unsigned int GetNumChildren(bool recursive);
	glm::vec3 LocalToWorld(const glm::vec3& vector);
	glm::vec3 WorldToLocal(const glm::vec3& vector);

	virtual void OnStart(){}
	virtual void OnUpdate(){}
	virtual void OnLateUpdate(){}
	virtual void OnRender(){}
	virtual void OnEnd(){}
	virtual void OnCollision(const CollisionData& data){}

};

}
