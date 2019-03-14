#include "Bullet.h"
#include "Ledlib/Time.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Physics/PolygonCollider.h"

void Bullet::OnStart(){
	std::shared_ptr<PolygonCollider> collider = Collider::Create<PolygonCollider>("bullet");
	collider->SetBox(1.0f, 1.0f);
	SetCollider(collider);
}
void Bullet::OnUpdate(){
	Translate(velocity * Time::deltaTime);
	countdown -= Time::deltaTime;
	if(countdown <= 0.0f){
		Destroy();
	}
}
void Bullet::OnRender(){
	Gfx::SetDrawColor(1, 1, 0);
	Gfx::FillRect(0, 0, 1.0f, 1.0f);
}
void Bullet::OnEnd(){}
