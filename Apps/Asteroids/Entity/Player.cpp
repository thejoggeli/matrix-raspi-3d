#include "Player.h"
#include "Game2d.h"
#include "State/GameState.h"
#include "Ledlib2d/Physics/PolygonCollider.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Gfx/Path.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Time.h"

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
	if(ClientManager::IsKeyDown(KeyCode::B)){
		acceleration.x = -20.0f;
	}
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
