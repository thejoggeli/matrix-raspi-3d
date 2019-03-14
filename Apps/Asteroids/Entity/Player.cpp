#include "Player.h"
#include "Game2d.h"
#include "State/GameState.h"
#include "Ledlib2d/Physics/PolygonCollider.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Gfx/Path.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Scene.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Time.h"
#include "Entity/Bullet.h"

void Player::OnStart(){
	std::shared_ptr<PolygonCollider> collider = Collider::Create<PolygonCollider>("player");
	vertices.push_back(glm::vec2(3.5, 0));
	vertices.push_back(glm::vec2(-3.5, -1.5f));
	vertices.push_back(glm::vec2(-2.0, 0));
	vertices.push_back(glm::vec2(-3.5, 1.5f));
	for(int i = 0; i < vertices.size(); i++){
		collider->AddPoint(vertices[i].x, vertices[i].y);
	}
	collider->GenerateTriangleFan();
	collider->drawBoundingCircle = true;
	SetCollider(collider);
}
void Player::OnUpdate(){
	glm::vec3 acceleration = glm::vec3(0,0,0);
	if(ClientManager::IsKeyDown(KeyCode::A)){
		acceleration.x = 20.0f;
	}
/*	if(ClientManager::IsKeyDown(KeyCode::Down)){
		acceleration.x = -20.0f;
	} */
	if(ClientManager::IsKeyDown(KeyCode::Left)){
		Rotate(Time::deltaTime*2.5f);
	}
	if(ClientManager::IsKeyDown(KeyCode::Right)){
		Rotate(-Time::deltaTime*2.5f);
	}
	acceleration = GetWorldRotation() * acceleration;
	velocity += Time::deltaTime * acceleration;
	Translate(velocity * Time::deltaTime);
	// bounds
	Game::GetInstance()->GetState<GameState>()->Boundify(*this, GetCollider()->_boundingRadius);
	// fire bullets
	if(bulletCooldown < 0.0f){
		if(ClientManager::IsKeyDown(KeyCode::B)){
			std::shared_ptr<Bullet> bullet = GetScene()->CreateEntity<Bullet>("bullet");
			bullet->SetPosition(GetWorldPosition());
			bullet->SetRotation(GetWorldRotation());
			bullet->velocity = GetWorldRotation() * glm::vec3(50.0f, 0, 0);
			bullet->countdown = 2.0f;
			bulletCooldown = 0.075f;
		}
	} else {
		bulletCooldown -= Time::deltaTime;
	}
}
void Player::OnRender(){
	Gfx::SetDrawColor(1, 1, 1);
	Gfx::BeginPath();
	for(int i = 0; i < vertices.size(); i++){
		Gfx::LineTo(vertices[i].x, vertices[i].y);
	}
	Gfx::ClosePath();
	Gfx::FillPath();
}
void Player::OnEnd(){}
