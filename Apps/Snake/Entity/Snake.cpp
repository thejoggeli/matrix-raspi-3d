#include "Snake.h"
#include "SnakeSegment.h"
#include "Goodie.h"
#include "Ledlib2d/Scene.h"
#include "Ledlib/Time.h"
#include "Ledlib/Log.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib2d/Physics/CollisionData.h"
#include "Ledlib2d/Physics/Collider.h"

void Snake::OnStart(){
	std::shared_ptr<SnakeSegment> segment = GetScene()->CreateEntity<SnakeSegment>("snake-segment");
	head = segment;
	segment->SetParent(shared_from_this());
	segment->isHead = true;
	segment->depth = 0;
	segment->Rotate(1.0f);
	segment->SetSnake(std::static_pointer_cast<Snake>(shared_from_this()));
	for(int i = 0; i < 25; i++){
		Grow();
	}
}
void Snake::OnUpdate(){
	std::shared_ptr<SnakeSegment> head = this->head.lock();
	if(ClientManager::IsKeyDown(KeyCode::Left)){
		head->Rotate(Time::deltaTime*3);
	}
	if(ClientManager::IsKeyDown(KeyCode::Right)){
		head->Rotate(-Time::deltaTime*3);
	}
	head->Translate(head->rotation * glm::vec3(20.0f, 0.0f, 0.0f) * Time::deltaTime);
}
void Snake::OnRender(){}
void Snake::OnAfterRender(){}
void Snake::OnEnd(){}

std::shared_ptr<SnakeSegment> Snake::LastSegment(){
	std::shared_ptr<SnakeSegment> segment = head.lock();
	while(true){
		if(segment->next.expired()){
			break;
		}
		segment = segment->next.lock();
	}
	return segment;
}

void Snake::Grow(){
	auto last = LastSegment();
	last->Grow();
}

void Snake::OnCollision(const CollisionData &data){
	if(data.a->group == "snake-head"){
		if(data.b->group == "snake-head" || data.b->group == "snake-tail"){
			auto segment = std::static_pointer_cast<SnakeSegment>(data.b->GetEntity());
			if(segment->depth > 5){
				Destroy();
			}
		} else if(data.b->group == "goodie"){
			auto goodie = std::static_pointer_cast<Goodie>(data.b->GetEntity());
			Grow();
			goodie->Destroy();
		} else if(data.b->group == "wall"){
			Destroy();
		}
	} else if(data.a->group == "snake-tail"){
		if(data.b->group == "wall"){
			auto segment = std::static_pointer_cast<SnakeSegment>(data.a->GetEntity());
			segment->Cut();
		}
	}
}
