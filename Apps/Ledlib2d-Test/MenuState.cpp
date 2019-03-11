#include "MenuState.h"
#include "TestEntity.h"
#include "MenuState.h"
#include "TestGame.h"

#include "Ledlib2d/Resources/ResourceManager.h"
#include "Ledlib2d/Scene.h"
#include "Ledlib2d/Camera.h"
#include "Ledlib2d/Gfx/Renderer.h"
#include "Ledlib2d/Gfx/Text.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Transform.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Remote/Client.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Log.h"
#include "Ledlib/Math/Vector2f.h"
#include "Ledlib/Time.h"
#include <math.h>
#include "Ledlib2d/Physics/BoxCollider.h"
#include "Ledlib2d/Physics/PointCollider.h"
#include "Ledlib2d/Physics/PolygonCollider.h"

void MenuState::OnStart(){
	std::shared_ptr<Entity> e1 = GetScene()->CreateEntity<TestEntity>("e1");
	std::shared_ptr<Entity> e2 = GetScene()->CreateEntity<TestEntity>("e2");
	std::shared_ptr<Entity> e3 = GetScene()->CreateEntity<TestEntity>("e3", e2);
	e1->SetScale(2.0f);
	e2->Translate(16, 0);
	e3->Translate(0, -16);
	entities.push_back(e1);
	entities.push_back(e2);
	entities.push_back(e3);

//	GetScene()->SetDebugDrawEnabled(true);
	GetScene()->EnableCollision("default", "default");
/*	std::shared_ptr<PolygonCollider> collider = Collider::Create<PolygonCollider>("default");
	e2->SetCollider(collider);
	collider->SetBox(12,12); */

	std::shared_ptr<PolygonCollider> c1 = Collider::Create<PolygonCollider>("default");
	std::shared_ptr<PolygonCollider> c2 = Collider::Create<PolygonCollider>("default");
	c1->SetBox(12, 12);
	c2->SetBox(12, 12);
	c1->drawBoundingCircle = false;
	c2->drawBoundingCircle = false;
	e1->SetCollider(c1);
	e2->SetCollider(c2);
	e2->AddTag("bounce");

}

void MenuState::OnUpdate(){
	if(auto p = entities[0].lock()){
		p->Rotate(Time::deltaTime);
	}
	if(auto p = entities[1].lock()){
		p->SetScale(sinf(Time::sinceStart*5.0f)*0.5f + 1.25f);
		p->Translate(-Time::deltaTime*5.0f, 0);
		p->Rotate(-Time::deltaTime*0.5f);
	}
	if(auto p = entities[2].lock()){
	//	p->SetPosition(sinf(Time::sinceStart*2.5f)*10.0f, p->position.y);
		p->Rotate(-Time::deltaTime*0.25f);
	}
	if(ClientManager::OnKeyDown(KeyCode::Select)){
		Game::GetInstance()->SetState<MenuState>();
	}
	if(ClientManager::OnKeyDown(KeyCode::A)){
		if(auto p = entities[0].lock()){
			p->DestroyChildren(false);
		}
	}
	if(ClientManager::OnKeyDown(KeyCode::B)){
		if(auto p = entities[2].lock()){
			p->SetPosition(p->GetWorldPosition());
			p->SetRotation(p->GetWorldRotation());
			p->SetScale(p->GetWorldScale());
			p->SetParent(GetScene()->GetRoot());
		}
	}
	std::shared_ptr<Camera> camera = GetCamera();
	camera->SimpleJoystickMove();
	camera->SimpleArrowsRotate();
	camera->SimpleArrowsZoom();
}
void MenuState::OnBeforeRender(){}
