#include "MenuState.h"
#include "TestEntity.h"

#include "Ledlib2d/Scene/Scene.h"
#include "Ledlib2d/Scene/Camera.h"
#include "Ledlib2d/Gfx/Renderer.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Remote/Client.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Math/Vector2f.h"
#include "Ledlib/Time.h"
#include <math.h>

void MenuState::OnStart(){
	std::shared_ptr<Entity> e1 = GetScene()->CreateEntity<TestEntity>();
	std::shared_ptr<Entity> e2 = GetScene()->CreateEntity<TestEntity>(e1);
	std::shared_ptr<Entity> e3 = GetScene()->CreateEntity<TestEntity>(e2);
	e2->Translate(16, 0);
	e3->Translate(0, -16);
	entities.push_back(e1);
	entities.push_back(e2);
	e2->SetRotation(glm::angleAxis(Numbers::Pi/2, glm::vec3(0, 1, 0)));
	entities.push_back(e3);
	Gfx::SetDepthTestEnabled(true);
}

void MenuState::OnUpdate(){


	if(auto p = entities[0].lock()){
		p->Rotate(Time::deltaTime);
		p->SetRotation(glm::angleAxis(Time::sinceStart, glm::vec3(0, 1, 0)));
	}
	if(auto p = entities[1].lock()){
		p->Rotate(-Time::deltaTime);
		p->SetScale(sinf(Time::sinceStart*5.0f)*0.5f + 1.25f);
	}
	if(auto p = entities[2].lock()){
	//	p->SetPosition(sinf(Time::sinceStart*2.5f)*10.0f, p->position.y);
		p->Rotate(-Time::deltaTime*0.25f);
	}
/*	if(ClientManager::OnKeyDown(KeyCode::A)){
		if(auto p = entities[1].lock()){
			p->Destroy();
		}
	}
	if(ClientManager::OnKeyDown(KeyCode::B)){
		if(auto p = entities[2].lock()){
			p->SetParent(GetScene()->GetRoot());
		}
	} */
	std::shared_ptr<Entity> camera = this->GetCamera()->GetEntity();
	if(ClientManager::OnKeyDown(KeyCode::Start)){
		camera->SetPosition(0, 0);
		camera->SetRotation(0);
		camera->SetScale(1, 1);
	}
	if(ClientManager::IsKeyDown(KeyCode::Left)){
		camera->Rotate(Numbers::Pi * Time::deltaTime);
	}
	if(ClientManager::IsKeyDown(KeyCode::Right)){
		camera->Rotate(-Numbers::Pi * Time::deltaTime);
	}
	if(ClientManager::IsKeyDown(KeyCode::Up)){
		camera->SetScale(camera->scale.x - camera->scale.x * Time::deltaTime);
	}
	if(ClientManager::IsKeyDown(KeyCode::Down)){
		camera->SetScale(camera->scale.x + camera->scale.x * Time::deltaTime);
	}
	for(auto& client: ClientManager::GetAllCients()){
		if(client->IsKeyDown(KeyCode::LeftJoystick)){
			Vector2f joy = client->GetJoystickPosition(KeyCode::LeftJoystick);
			float angle = camera->GetAngle() + atan2f(joy.y, joy.x);
			glm::vec3 dir = glm::vec3(cosf(angle), sinf(angle), 0);
			camera->Translate(dir * Time::deltaTime * 20.0f * joy.GetLength());
			break;
		}
	}
}
