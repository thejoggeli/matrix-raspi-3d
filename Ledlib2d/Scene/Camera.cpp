#include "Camera.h"
#include "Entity.h"
#include "Ledlib/Log.h"
#include "Ledlib/Remote/Client.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Time.h"

namespace Ledlib {

Camera::Camera(){}

std::shared_ptr<Camera> Camera::Create(const std::shared_ptr<Entity>& entity){
	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	camera->entity = entity;
	return camera;
}
std::shared_ptr<Entity> Camera::GetEntity(){
	if(auto p = entity.lock()){
		return p;
	}
	Log(LOG_ERROR, "Camera", "Entity expired");
	return nullptr;
}

void Camera::SimpleMove(){
	std::shared_ptr<Entity> entity = GetEntity();
	for(auto& client: ClientManager::GetAllCients()){
		if(client->IsKeyDown(KeyCode::LeftJoystick)){
			Vector2f joy = client->GetJoystickPosition(KeyCode::LeftJoystick);
			glm::vec3 dir = entity->rotation * glm::vec3(joy.x, joy.y, 0);
			entity->Move(dir * Time::deltaTime * 20.0f);
			break;
		}
	}
}
void Camera::SimpleRotate(){
	std::shared_ptr<Entity> entity = GetEntity();
	if(ClientManager::IsKeyDown(KeyCode::Left)){
		entity->Rotate(Numbers::Pi * Time::deltaTime);
	}
	if(ClientManager::IsKeyDown(KeyCode::Right)){
		entity->Rotate(-Numbers::Pi * Time::deltaTime);
	}
}
void Camera::SimpleZoom(){
	std::shared_ptr<Entity> entity = GetEntity();
	if(ClientManager::IsKeyDown(KeyCode::Up)){
		entity->SetScale(entity->scale.x - entity->scale.x * Time::deltaTime);
	}
	if(ClientManager::IsKeyDown(KeyCode::Down)){
		entity->SetScale(entity->scale.x + entity->scale.x * Time::deltaTime);
	}
}

}

