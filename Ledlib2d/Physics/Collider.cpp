#include "Collider.h"
#include "Ledlib/Log.h"
#include "../Entity.h"

#include "../Gfx/Transform.h"
#include "../Gfx/Draw.h"

namespace Ledlib {

int Collider::updateCounter = 0;

Collider::Collider(){}
Collider::~Collider(){}

void Collider::Setup(const std::string& group){
	_position = glm::vec2(0,0);
	_rotation = 0.0f;
	_scale = glm::vec2(1,1);
	_group = group;
	_needsUpdateFlag = true;
	drawBoundingCircle = false;
	drawCenter = false;
	OnSetup();
}

void Collider::UpdateTransform(){
	updateCounter++;
	_needsUpdateFlag = false;
	std::shared_ptr<Entity> p = GetEntity();
	_position = p->GetWorldPosition();
	_rotation = p->GetWorldAngle();
	_scale = p->GetWorldScale();
	OnUpdateTransform();
}

void Collider::SetNeedsUpdate(){
	_needsUpdateFlag = true;
}

void Collider::SetEntity(const std::shared_ptr<Entity>& entity){
	_entity = entity;
	SetNeedsUpdate();
}
std::shared_ptr<Entity> Collider::GetEntity(){
	if(auto p = _entity.lock()){
		return p;
	}
	Log(LOG_ERROR, "Collider", "Entity expired");
	return nullptr;
}

void Collider::DebugDraw(){
/*	Gfx::Translate(_position.x, _position.y);
	Gfx::Rotate(_rotation);
	Gfx::Scale(_scale.x, _scale.y); */
	if(drawBoundingCircle){
		Gfx::SetDrawColor(0, 1, 1, 0.5f);
		Gfx::FillCircle(_position.x, _position.y, _boundingRadius, 16);
	}
	OnDebugDraw();
	if(drawCenter){
		Gfx::SetDrawColor(1, 1, 0);
		Gfx::FillRect(_position.x, _position.y, 2, 2);
	}
}

}
