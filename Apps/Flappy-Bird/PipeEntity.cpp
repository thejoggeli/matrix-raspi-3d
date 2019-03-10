#include "PipeEntity.h"
#include "Ledlib/Log.h"
#include "Ledlib2d/Resources/ResourceManager.h"
#include "Ledlib2d/Gfx/Bitmaps.h"
#include "Ledlib2d/Gfx/Transform.h"
#include "Ledlib2d/Physics/BoxCollider.h"
#include "Ledlib2d/Scene.h"

PipeEntity::PipeEntity(){}

void PipeEntity::OnStart(){
	bitmap = ResourceManager::GetBitmap("pipe");
	std::shared_ptr<BoxCollider> collider = Collider::Create<BoxCollider>("pipe");
	std::shared_ptr<Entity> colliderEntity = GetScene()->CreateEntity<Entity>("pipe-collider", shared_from_this());
	colliderEntity->SetCollider(collider);
	collider->SetSize(width, 128);
	colliderEntity->Translate(0, -128/2+height/2);
}

void PipeEntity::OnUpdate(){

}

void PipeEntity::OnRender(){
	Gfx::DrawBitmap(bitmap, 0, 0);
}
