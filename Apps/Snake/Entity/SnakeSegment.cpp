#include "SnakeSegment.h"
#include "Snake.h"
#include "Goodie.h"
#include "Game2d.h"
#include "State/GameState.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib/Time.h"
#include "Ledlib2d/Scene.h"
#include "Ledlib2d/Physics/PolygonCollider.h"
#include <cmath>

void SnakeSegment::OnStart(){
	radius = 1.5f;
	nextDistance = 1.0f;
}
void SnakeSegment::OnUpdate(){
	if(!next.expired()){
		auto next = this->next.lock();
		glm::vec3 dir = glm::normalize(position - next->position);
		float angle = std::atan2(dir.y, dir.x);
		next->SetRotation(angle);
		next->SetPosition(position - dir * nextDistance);
	}
	if(isLoose){
		despawnTimer -= Time::deltaTime * 0.1f;
		if(despawnTimer <= 0.3f){
			Destroy();
			if(depth%3==0){
				auto state = Game2d::GetInstance()->GetState<GameState>();
				auto goodie = state->GetScene()->CreateEntity<Goodie>("goodie");
				goodie->SetPosition(GetWorldPosition());
				state->goodies.Add(goodie);
			}
		}
	}
}
void SnakeSegment::OnRender(){
	if(isHead){
		ColorRgb& color = snake.lock()->color;
		Gfx::SetDrawColor(color.r, color.g, color.b, despawnTimer);
	//	Gfx::FillCircle(0, 0, radius);
		Gfx::FillRect(0, 0, radius*2, radius*2);
		Gfx::SetDrawColor(1, 1, 1);
		Gfx::FillRect(1.25f, -1.25f, 1.0f, 1.0f);
		Gfx::FillRect(1.25f, +1.25f, 1.0f, 1.0f);
	} else {
		ColorRgb& color = snake.lock()->color;
		Gfx::SetDrawColor(color.r, color.g, color.b, despawnTimer);
		Gfx::FillRect(0, 0, radius*2, radius*2);
	}
}
void SnakeSegment::OnExit(){}

void SnakeSegment::SetSnake(const std::shared_ptr<Snake>& snake){
	this->snake = snake;
	std::string group = isHead ? "snake-head" : "snake-tail";
	auto collider = Collider::Create<PolygonCollider>(group);
	collider->SetBox(radius*2, radius*2);
	SetCollider(collider);
	SetPropagateCollisions(true, false);
}
void SnakeSegment::Grow(){
	if(!next.expired()) return;
	auto segment = GetScene()->CreateEntity<SnakeSegment>("snake-segment");
	segment->SetSnake(snake.lock());
	segment->isHead = false;
	glm::vec3 dir = rotation * glm::vec3(1, 0, 0);
	segment->SetPosition(position - dir*nextDistance);
	segment->SetParent(snake.lock());
	segment->SetRotation(rotation);
	segment->depth = depth+1;
	next = segment;
}
void SnakeSegment::Cut(){
	Destroy();
	if(!next.expired()){
		next.lock()->SetLoose();
	}
}
void SnakeSegment::SetLoose(){
	isLoose = true;
	if(!next.expired()){
		next.lock()->SetLoose();
	}
}
