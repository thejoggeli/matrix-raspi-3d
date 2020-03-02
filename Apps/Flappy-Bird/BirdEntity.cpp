#include "BirdEntity.h"
#include "Ledlib2d/Physics/CollisionData.h"
#include "Ledlib2d/Physics/PolygonCollider.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Bitmaps.h"
#include "Ledlib2d/Gfx/Draw.h"
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
	// collider
	std::shared_ptr<Entity> colliderEntity = GetScene()->CreateEntity<Entity>("bird-collider", shared_from_this());
	std::shared_ptr<PolygonCollider> box = Collider::Create<PolygonCollider>("bird");
	colliderEntity->SetCollider(box);
	colliderEntity->SetPropagateCollisions(true, false);
	colliderEntity->Translate(-0.5f, 0);
	box->SetCircle(0.65f, 16);
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

	Entity* camera = Game::GetCameraEntityFromState().get();
	SetPosition(camera->position.x + Gfx::left/2, position.y + velocity.y * Time::deltaTime);
	if(this->position.y > Gfx::top*3 || this->position.y < Gfx::bottom*3){
		this->Destroy();
		RemoteSfx::PlaySound(0, "die");
		for(int i = 0; i < 25; i++){
			auto blood = GetScene()->CreateEntity<FlappyBlood>("blood");
			float white = Numbers::Random(0.0f, 0.2f);
			blood->color = ColorRgb(Numbers::Random(0.8f, 1.0f), white, white);
			blood->size = Numbers::Random(0, 7) == 0 ? 1.0f : 2.0f;
			blood->velocity = glm::vec2(Numbers::Random(-25.0f, 25.0f), Numbers::Random(60.0f, 120.0f));
			blood->acceleration = glm::vec2(0, -150.0f);
			blood->SetPosition(this->position);
			blood->angularVelocity = Numbers::Random(-2.0f, 2.0f);
		}
	}
}

void BirdEntity::OnRender(){
	Gfx::DrawBitmap(bitmap, 0, 0);
}

void BirdEntity::OnCollision(const CollisionData &data){
	Destroy();
	RemoteSfx::PlaySound(0, "bump");
	for(int i = 0; i < 25; i++){
		auto blood = GetScene()->CreateEntity<FlappyBlood>("blood");
		float white = Numbers::Random(0.0f, 0.2f);
		blood->color = ColorRgb(Numbers::Random(0.8f, 1.0f), white, white);
		blood->size = Numbers::Random(0, 7) == 0 ? 1.0f : 2.0f;
		blood->velocity = glm::vec2(Numbers::Random(-50.0f, 50.0f), Numbers::Random(-30.0f, 60.0f));
		blood->acceleration = glm::vec2(0, -150.0f);
		blood->SetPosition(this->position);
		blood->angularVelocity = Numbers::Random(-2.0f, 2.0f);
	}
}

void FlappyBlood::OnUpdate(){
	velocity += acceleration * Time::deltaTime;
	Rotate(angularVelocity*Time::deltaTime);
	Translate(velocity*Time::deltaTime);
	if(position.y < Gfx::bottom*10.0f){
		Destroy();
	}
}

void FlappyBlood::OnRender(){
	Gfx::SetDrawColor(color);
	Gfx::FillRect(0, 0, size, size);
}
