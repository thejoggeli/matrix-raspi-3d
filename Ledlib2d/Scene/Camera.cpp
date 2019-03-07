#include "Camera.h"
#include "Ledlib/Log.h"

namespace Ledlib {

Camera::Camera(){}

std::shared_ptr<Camera> Camera::Create(){
	return std::make_shared<Camera>();
}

void Camera::SetEntity(std::shared_ptr<Entity> entity){
	this->entity = entity;
}
std::shared_ptr<Entity> Camera::GetEntity(){
	if(auto p = entity.lock()){
		return p;
	}
	Log(LOG_ERROR, "Camera", "Entity expired");
	return nullptr;
}

}

