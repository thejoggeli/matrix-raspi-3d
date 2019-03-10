#include "Entity.h"
#include "Ledlib/Log.h"
#include "Scene.h"
#include "Camera.h"
#include "Component.h"
#include "../Physics/PhysicsBody.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

using namespace std;
using namespace glm;

namespace Ledlib {

int Entity::idCounter;
int Entity::aliveCounter;

Entity::Entity(){
	_id = idCounter++;
	aliveCounter++;
	Log(LOG_DEBUG, "Entity", iLog << "Entity created (id=" << _id << "), " << aliveCounter << " alive");
}
Entity::~Entity(){
	aliveCounter--;
	Log(LOG_DEBUG, "Entity", iLog << "Entity destroyed (id=" << _id << "), " << aliveCounter << " alive");
}

std::shared_ptr<Entity> Entity::Init(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Scene>& scene, const std::shared_ptr<Entity>& parent){
	entity->_scene = scene;
	if(parent == nullptr){
		std::shared_ptr<Entity> root = scene->GetRoot();
		entity->_parent = root;
		root->_children.push_back(entity);
	} else {
		entity->_parent = parent;
		parent->_children.push_back(entity);
	}
	scene->OnEntityCreated(entity);
	return entity;
}
std::shared_ptr<Entity> Entity::CreateRoot(const std::shared_ptr<Scene> &scene){
	std::shared_ptr<Entity> entity = std::make_shared<Entity>();
	entity->_scene = scene;
	scene->OnEntityCreated(entity);
	return entity;
}

std::shared_ptr<Scene> Entity::GetScene(){
	if(auto p = _scene.lock()){
		return p;
	}
	Log(LOG_ERROR, "Entity", "Scene expired");
	return nullptr;
}

void Entity::CreateCamera(){
	_camera = Camera::Create(shared_from_this());
}
std::shared_ptr<Camera> Entity::GetCamera(){
	return _camera;
}

void Entity::SetPosition(float x, float y, float z){
	_position.x = x;
	_position.y = y;
	_position.z = z;
	SetNeedsUpdate();
}
void Entity::SetPosition(const vec3& v){
	_position = v;
	SetNeedsUpdate();
}
void Entity::Translate(float x, float y, float z){
	_position.x += x;
	_position.y += y;
	_position.z += z;
	SetNeedsUpdate();
}
void Entity::Translate(const glm::vec3 &v){
	_position += v;
	SetNeedsUpdate();
}
void Entity::Move(const glm::vec3& v){
	_position += _rotation * v;
	SetNeedsUpdate();
}

void Entity::SetScale(float s){
	_scale.x = s;
	_scale.y = s;
	SetNeedsUpdate();
}
void Entity::SetScale(float x, float y, float z){
	_scale.x = x;
	_scale.y = y;
	_scale.z = z;
	SetNeedsUpdate();
}
void Entity::SetScale(const vec3& v){
	_scale = v;
	SetNeedsUpdate();
}
void Entity::Scale(float s){
	_scale.x *= s;
	_scale.y *= s;
	SetNeedsUpdate();
}
void Entity::Scale(float x, float y, float z){
	_scale.x *= x;
	_scale.y *= y;
	_scale.z *= z;
	SetNeedsUpdate();
}
void Entity::Scale(const glm::vec3& v){
	_scale.x *= v.x;
	_scale.y *= v.y;
	_scale.z *= v.z;
	SetNeedsUpdate();
}

void Entity::SetRotation(float z){
	_rotation = glm::angleAxis(z, vec3(0, 0, 1));
	SetNeedsUpdate();
}
void Entity::SetRotation(const mat4& rotation){
	_rotation = rotation;
	SetNeedsUpdate();
}
void Entity::SetRotation(const quat& rotation){
	_rotation = rotation;
	SetNeedsUpdate();
}
void Entity::Rotate(float z){
	_rotation = glm::rotate(_rotation, z, vec3(0,0,1));
	SetNeedsUpdate();
}
void Entity::Rotate(const glm::quat& rotation){
	_rotation = _rotation * rotation;
}
float Entity::GetAngle(){
	return glm::angle(_rotation);
}

void Entity::AddChild(std::shared_ptr<Entity> child){
	std::shared_ptr<Entity> childsParent = child->GetParent();
	if(childsParent){
		childsParent->_children.erase(std::remove(childsParent->_children.begin(), childsParent->_children.end(), child), childsParent->_children.end());
	}
	_children.push_back(child);
	child->SetNeedsUpdate();
}

std::vector<std::shared_ptr<Entity>>& Entity::GetChildren(){
	return _children;
}

std::shared_ptr<Entity> Entity::GetParent(){
	if(auto p = _parent.lock()){
		return p;
	}
	Log(LOG_ERROR, "Entity", "Parent expired");
	return nullptr;
}

void Entity::SetParent(std::shared_ptr<Entity> parent){
	if(parent == GetParent()){
		Log(LOG_DEBUG, "Entity", "Same parent");
		return;
	}
	if(!parent){
		if(_destroyed){
			auto& pChildren = GetParent()->_children;
			pChildren.erase(std::remove(pChildren.begin(), pChildren.end(), shared_from_this()), pChildren.end());
			Log("time to destroy");
		} else {
			Destroy();
		}
	} else {
		// add this entity to new parent
		parent->_children.push_back(shared_from_this());
		// remove this entity from old parent's children list
		auto& pChildren = GetParent()->_children;
		pChildren.erase(std::remove(pChildren.begin(), pChildren.end(), shared_from_this()), pChildren.end());
		// set new parent
		_parent = parent;
		// update matrix
		SetNeedsUpdate();
	}
}

void Entity::Destroy(){
	if(_destroyed) return;
	_destroyed = true;
	GetScene()->OnEntityDestroyed(shared_from_this());
}
void Entity::DestroyChildren(bool recursive){
	for(auto& child: GetChildren()){
		child->Destroy();
		if(recursive){
			child->DestroyChildren(true);
		}
	}
}

void Entity::SetNeedsUpdate(){
	_needsUpdate = true;
	for(auto& child: _children){
		child->SetNeedsUpdate();
	}
}

mat4& Entity::GetWorldMatrix(){
	if(_needsUpdate){
		UpdateWorldMatrix();
	}
	return _worldMatrix;
}

mat4& Entity::GetMatrix(){
	if(_needsUpdate){
		UpdateMatrix();
	}
	return _matrix;
}

quat& Entity::GetWorldRotation(){
	if(_needsUpdate){
		UpdateWorldMatrix();
	}
	return _worldRotation;
}

void Entity::UpdateMatrix(){
	_matrix = mat4(1.0f);
	_matrix = glm::translate(_matrix, position);
	_matrix = _matrix * glm::toMat4(_rotation);
	_matrix = glm::scale(_matrix, _scale);
}

void Entity::UpdateWorldMatrix(){
	mat4 matrix = GetMatrix();
	std::shared_ptr<Entity> parent = GetParent();
	if(parent == GetScene()->GetRoot()){
		_worldMatrix = matrix;
		_worldRotation = _rotation;
	} else {
		_worldMatrix = parent->GetWorldMatrix() * matrix;
		_worldRotation = parent->GetWorldRotation() * _rotation;
	}
}

vec3 Entity::GetWorldPosition(){
	if(_needsUpdate){
		UpdateWorldMatrix();
	}
	return vec3(_worldMatrix[3]);
}
vec3 Entity::GetWorldScale(){
	if(_needsUpdate){
		UpdateWorldMatrix();
	}
	// might be wrong
	// https://math.stackexchange.com/questions/237369/given-this-transformation-matrix-how-do-i-decompose-it-into-translation-rotati
	return GetParent()->scale * scale;
}

unsigned int Entity::GetNumChildren(bool recursive){
	if(recursive){
		unsigned int count = _children.size();
		for(auto& child: _children){
			count += child->GetNumChildren(true);
		}
		return count;
	}
	return _children.size();
}
vec3 Entity::LocalToWorld(const glm::vec3& vector){
	return vec3(GetWorldMatrix() * vec4(vector, 1));
}
vec3 Entity::WorldToLocal(const glm::vec3& vector){
	return vec3(glm::inverse(GetWorldMatrix()) * vec4(vector, 1));
}

}
