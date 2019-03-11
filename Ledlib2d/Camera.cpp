#include "Camera.h"
#include "Entity.h"
#include "Ledlib/Log.h"
#include "Ledlib/Remote/Client.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Time.h"
#include "Gfx/Transform.h"

namespace Ledlib {

Camera::Camera(){}

std::shared_ptr<Camera> Camera::Create(){
	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	return camera;
}
std::shared_ptr<Entity> Camera::GetEntity(){
	if(auto p = entity.lock()){
		return p;
	}
	Log(LOG_ERROR, "Camera", "Entity expired");
	return nullptr;
}
void Camera::SetEntity(const std::shared_ptr<Entity>& entity){
	this->entity = entity;
}

void Camera::SimpleJoystickMove(float sensitivity){
	for(auto& client: ClientManager::GetAllCients()){
		if(client->IsKeyDown(KeyCode::LeftJoystick)){
			std::shared_ptr<Entity> entity = GetEntity();
			Vector2f joy = client->GetJoystickPosition(KeyCode::LeftJoystick);
			glm::vec3 dir = entity->GetWorldRotation() * glm::vec3(joy.x, joy.y, 0);
			dir *= sensitivity;
			entity->Translate(dir * Time::deltaTime);
			break;
		}
	}
}
void Camera::SimpleJoystickRotate(float sensitivity){
	for(auto& client: ClientManager::GetAllCients()){
		if(client->IsKeyDown(KeyCode::LeftJoystick)){
			std::shared_ptr<Entity> entity = GetEntity();
			Vector2f joy = client->GetJoystickPosition(KeyCode::LeftJoystick);
			joy *= sensitivity;
			entity->Rotate(Numbers::Pi * Time::deltaTime * -joy.x);
			break;
		}
	}
}
void Camera::SimpleJoystickZoom(float sensitivity){
	for(auto& client: ClientManager::GetAllCients()){
		if(client->IsKeyDown(KeyCode::LeftJoystick)){
			std::shared_ptr<Entity> entity = GetEntity();
			Vector2f joy = client->GetJoystickPosition(KeyCode::LeftJoystick);
			joy *= sensitivity;
			entity->SetScale(entity->scale.x + entity->scale.x * Time::deltaTime * -joy.y);
			break;
		}
	}
}
void Camera::SimpleArrowsMove(float sensitivity){
	glm::vec3 dir(0, 0, 0);
	if(ClientManager::IsKeyDown(KeyCode::Left)) dir.x = -1;
	if(ClientManager::IsKeyDown(KeyCode::Right)) dir.x = 1;
	if(ClientManager::IsKeyDown(KeyCode::Up)) dir.y = 1;
	if(ClientManager::IsKeyDown(KeyCode::Down)) dir.y = -1;
	if(glm::length(dir) != 0.0f){
		std::shared_ptr<Entity> entity = GetEntity();
		dir = glm::normalize(dir);
		dir = entity->GetWorldRotation() * dir;
		entity->Translate(dir * Time::deltaTime * sensitivity);
	}
}
void Camera::SimpleArrowsRotate(float sensitivity){
	if(ClientManager::IsKeyDown(KeyCode::Left)){
		std::shared_ptr<Entity> entity = GetEntity();
		entity->Rotate(Numbers::Pi * Time::deltaTime * sensitivity);
	}
	if(ClientManager::IsKeyDown(KeyCode::Right)){
		std::shared_ptr<Entity> entity = GetEntity();
		entity->Rotate(-Numbers::Pi * Time::deltaTime * sensitivity);
	}
}
void Camera::SimpleArrowsZoom(float sensitivity){
	if(ClientManager::IsKeyDown(KeyCode::Up)){
		std::shared_ptr<Entity> entity = GetEntity();
		entity->SetScale(entity->scale.x - entity->scale.x * Time::deltaTime * sensitivity);
	}
	if(ClientManager::IsKeyDown(KeyCode::Down)){
		std::shared_ptr<Entity> entity = GetEntity();
		entity->SetScale(entity->scale.x + entity->scale.x * Time::deltaTime * sensitivity);
	}
}
void Camera::ApplyTransform(){
	Gfx::Transform(glm::inverse(GetEntity()->GetWorldMatrix()));
}

}

