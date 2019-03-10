#include "BirdEntity.h"
#include "Ledlib2d/Physics/CollisionData.h"
#include "Ledlib2d/Physics/BoxCollider.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Bitmaps.h"
#include "Ledlib/Sfx/RemoteSfx.h"
#include "Ledlib/Remote/Client.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Time.h"
#include "Ledlib2d/Scene.h"
#include "Ledlib2d/State.h"
#include "Ledlib2d/Game.h"

BirdEntity::BirdEntity(){}

void BirdEntity::OnStart(){
	SetPosition(0, 0);
	flapVelocity = 48;
	velocity = glm::vec2(0, flapVelocity);
	acceleration = glm::vec2(0, -200);
	std::shared_ptr<BoxCollider> box = Collider::Create<BoxCollider>("bird");
	box->SetSize(4*0.666f, 3*0.666f);
	SetCollider(box);
}

void BirdEntity::OnUpdate(){
	for(auto &client: ClientManager::GetAllCients()){
		if(client->OnKeyDown(KeyCode::A)){
			velocity.y = flapVelocity;
		//	RemoteSfx::PlaySound(client->id, "jump");
			SetRotation(Numbers::Pi/2);
		}
	}
	velocity.y += acceleration.y * Time::deltaTime;
	if(velocity.y < -16){
		SetRotation(-Numbers::Pi/2);
	} else if(velocity.y < 16){
		SetRotation(0);
	} else {
		SetRotation(Numbers::Pi/2);
	}

	Entity* camera = Game::GetInstance()->GetState()->GetCameraEntity().get();
	SetPosition(camera->position.x + Gfx::left/2, position.y + velocity.y * Time::deltaTime);
	if(this->position.y > Gfx::top*3 || this->position.y < Gfx::bottom*3){
		this->Destroy();
		RemoteSfx::PlaySound(0, "die");
	}
}

void BirdEntity::OnRender(){
	Gfx::DrawBitmap(bitmap, 0, 0);
}

void BirdEntity::OnCollision(const CollisionData &data){
	Destroy();
	RemoteSfx::PlaySound(0, "bump");
}
