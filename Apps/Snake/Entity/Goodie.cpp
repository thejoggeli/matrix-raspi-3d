#include "Goodie.h"
#include "Ledlib2d/Physics/PolygonCollider.h"
#include "Ledlib2d/Gfx/Draw.h"

void Goodie::OnStart(){
	radius = 1.0f;
	auto collider = Collider::Create<PolygonCollider>("goodie");
	collider->SetBox(radius*2, radius*2);
	SetCollider(collider);
}
void Goodie::OnUpdate(){

}
void Goodie::OnRender(){
	Gfx::SetDrawColor(1,1,0);
	Gfx::FillRect(0,0,radius*2,radius*2);
}
void Goodie::OnExit(){}
