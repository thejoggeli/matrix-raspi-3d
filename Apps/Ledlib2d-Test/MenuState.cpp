#include "MenuState.h"
#include "TestEntity.h"
#include "MenuState.h"
#include "TestGame.h"

#include "Ledlib2d/Resources/ResourceManager.h"
#include "Ledlib2d/Scene/Scene.h"
#include "Ledlib2d/Scene/Camera.h"
#include "Ledlib2d/Gfx/Renderer.h"
#include "Ledlib2d/Gfx/Text.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Transform.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Remote/Client.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Math/Vector2f.h"
#include "Ledlib/Time.h"
#include <math.h>

static std::shared_ptr<Scene> scene;
static std::weak_ptr<Entity> cam;

void MenuState::OnStart(){

	scene = Scene::Create();
	scene->CreateEntity<TestEntity>();

	std::shared_ptr<Entity> c = scene->CreateEntity<Entity>();
	c->CreateCamera();
	cam = c;

	std::shared_ptr<Entity> e1 = GetScene()->CreateEntity<TestEntity>();
	std::shared_ptr<Entity> e2 = GetScene()->CreateEntity<TestEntity>(e1);
	std::shared_ptr<Entity> e3 = GetScene()->CreateEntity<TestEntity>(e2);
	std::shared_ptr<Entity> e4 = GetScene()->CreateEntity<TestEntity>(e3);
	std::shared_ptr<Entity> e5 = GetScene()->CreateEntity<TestEntity>(e3);
	e2->Translate(16, 0);
	e3->Translate(0, -16);
	e4->Translate(0, -16);
	entities.push_back(e1);
	entities.push_back(e2);
	entities.push_back(e3);
	entities.push_back(e4);
	for(int i = 0; i < 1; i++){
		std::shared_ptr<Entity> e = GetScene()->CreateEntity<TestEntity>(e1);
		e->Translate(-(i+1)*16, 0);
		entities.push_back(e);
	}
	Gfx::SetAutoClear(false);
}

void MenuState::OnUpdate(){
	if(auto p = entities[0].lock()){
		p->Rotate(Time::deltaTime);
	}
	if(auto p = entities[1].lock()){
		p->SetScale(sinf(Time::sinceStart*5.0f)*0.5f + 1.25f);
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
	camera->SimpleMove();
	camera->SimpleRotate();
	camera->SimpleZoom();
	scene->Update();
}
void MenuState::OnBeforeRender(){
	Gfx::Clear();
}
void MenuState::OnRender(){
	Gfx::Render(scene.get(), cam.lock()->GetCamera().get());
	Gfx::SetTextColor(1, 0, 0);
	Gfx::SetTextPosition(TextAlign::Center, TextBaseline::Middle);
	Gfx::SetFont(ResourceManager::GetFont("font-1"));
	Gfx::Save();
	Gfx::Translate(16, -12);
	Gfx::Rotate(entities[1].lock()->GetWorldRotation());
	Gfx::DrawText("hello:)", 0, 0);
	Gfx::Restore();
}
