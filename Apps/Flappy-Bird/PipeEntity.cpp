#include "PipeEntity.h"
#include "Ledlib/Log.h"
#include "Ledlib/Time.h"
#include "Ledlib2d/Resources/ResourceManager.h"
#include "Ledlib2d/Gfx/Bitmaps.h"
#include "Ledlib2d/Gfx/Transform.h"
#include "Ledlib2d/Physics/PolygonCollider.h"
#include "Ledlib2d/Scene.h"
#include <cmath>

PipeEntity::PipeEntity(){}

void PipeEntity::OnStart(){
	bitmap = ResourceManager::GetBitmap("pipe");
	std::shared_ptr<PolygonCollider> collider = Collider::Create<PolygonCollider>("pipe");
	std::shared_ptr<Entity> colliderEntity = GetScene()->CreateEntity<Entity>("pipe-collider", shared_from_this());
	colliderEntity->SetCollider(collider);
	collider->SetBox(width, 128);
	colliderEntity->Translate(0, -128/2+height/2);
}

void PipeEntity::OnUpdate(){
	if(moving) {
		float off = std::sin(Time::sinceStart*2.0f+position.x) * 8.0f;
		SetPosition(originalPosition.x, originalPosition.y + off);
	}
	if(rotating) {
		float off = std::sin(Time::sinceStart+position.x) * 0.25f;
		SetRotation(originalAngle + off);
	}
}

void PipeEntity::OnRender(){
	Gfx::DrawBitmap(bitmap, 0, 0);
}
