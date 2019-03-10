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

void Camera::SimpleJoystickMove(){
	for(auto& client: ClientManager::GetAllCients()){
		if(client->IsKeyDown(KeyCode::LeftJoystick)){
			std::shared_ptr<Entity> entity = GetEntity();
			Vector2f joy = client->GetJoystickPosition(KeyCode::LeftJoystick);
			glm::vec3 dir = entity->GetWorldRotation() * glm::vec3(joy.x, joy.y, 0);
			entity->Translate(dir * Time::deltaTime * 20.0f);
			break;
		}
	}
}
void Camera::SimpleJoystickRotate(){
	for(auto& client: ClientManager::GetAllCients()){
		if(client->IsKeyDown(KeyCode::LeftJoystick)){
			std::shared_ptr<Entity> entity = GetEntity();
			Vector2f joy = client->GetJoystickPosition(KeyCode::LeftJoystick);
			entity->SetScale(entity->scale.x + entity->scale.x * Time::deltaTime * -joy.y);
			break;
		}
	}
}
void Camera::SimpleJoystickZoom(){
	for(auto& client: ClientManager::GetAllCients()){
		if(client->IsKeyDown(KeyCode::LeftJoystick)){
			std::shared_ptr<Entity> entity = GetEntity();
			Vector2f joy = client->GetJoystickPosition(KeyCode::LeftJoystick);
			entity->Rotate(Numbers::Pi * Time::deltaTime * -joy.x);
			break;
		}
	}
}
void Camera::SimpleArrowsMove(){
	// todo
}
void Camera::SimpleArrowsRotate(){
	if(ClientManager::IsKeyDown(KeyCode::Left)){
		std::shared_ptr<Entity> entity = GetEntity();
		entity->Rotate(Numbers::Pi * Time::deltaTime);
	}
	if(ClientManager::IsKeyDown(KeyCode::Right)){
		std::shared_ptr<Entity> entity = GetEntity();
		entity->Rotate(-Numbers::Pi * Time::deltaTime);
	}
}
void Camera::SimpleArrowsZoom(){
	if(ClientManager::IsKeyDown(KeyCode::Up)){
		std::shared_ptr<Entity> entity = GetEntity();
		entity->SetScale(entity->scale.x - entity->scale.x * Time::deltaTime);
	}
	if(ClientManager::IsKeyDown(KeyCode::Down)){
		std::shared_ptr<Entity> entity = GetEntity();
		entity->SetScale(entity->scale.x + entity->scale.x * Time::deltaTime);
	}
}
void Camera::ApplyTransform(){
	Gfx::Transform(glm::inverse(GetEntity()->GetWorldMatrix()));
}

}

