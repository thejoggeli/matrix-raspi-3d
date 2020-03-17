#include "GameState.h"
#include "Game2d.h"
#include "Entity/Snake.h"
#include "Entity/SnakeSegment.h"
#include "Entity/Goodie.h"
#include "Entity/Wall.h"
#include "Ledlib2d/Scene.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Log.h"
#include "Ledlib2d/Gfx/Gfx.h"

void GameState::OnStart(){
//	GetScene()->SetDebugDrawEnabled(true);

	GetScene()->EnableCollision("snake-head", "snake-head");
	GetScene()->EnableCollision("snake-head", "snake-tail");
	GetScene()->EnableCollision("snake-head", "goodie");
	GetScene()->EnableCollision("snake-head", "wall");
	GetScene()->EnableCollision("snake-tail", "wall");

	std::shared_ptr<Snake> snake;

	snake = GetScene()->CreateEntity<Snake>("snake");
	snake->head.lock()->Rotate(1.0f);
	snake->color = ColorRgb::RED;
	for(int i = 0; i < 25; i++){
		snake->Grow();
	}
	snakes.Add(snake);

/*	snake = GetScene()->CreateEntity<Snake>("snake");
	snake->head.lock()->Rotate(0.0f);
	snake->color = ColorRgb::GREEN;
	snake->head.lock()->SetPosition(0, -30, 0);
	for(int i = 0; i < 15; i++){
		snake->Grow();
	}
	snakes.Add(snake); */

	auto wall = GetScene()->CreateEntity<Wall>("wall");
	wall->AddVertex(-16, -24);
	wall->AddVertex(-16, 24);
	wall->AddVertex(-32, 0);
	wall->Build();
	wall->Rotate(-1.0f);
	walls.Add(wall);

	wall = GetScene()->CreateEntity<Wall>("wall");
	wall->AddVertex(30, 0);
	wall->AddVertex(24, -24);
	wall->AddVertex(18, 24);
	wall->AddVertex(36, 20);
	wall->AddVertex(50, 0);
	wall->AddVertex(55, -10);
	wall->Build();
	wall->Translate(10, 0, 0);
	walls.Add(wall);

}
void GameState::OnUpdate(){

	goodies.RemoveExpired();
	snakes.RemoveExpired();
	if(snakes.Size() < 1){
		Game2d::GetInstance()->SetState<GameState>();
		return;
	}

	// spawn new goodies
	while(goodies.Size() < 100){
		auto goodie = GetScene()->CreateEntity<Goodie>("goodie");
		float bounds = 128.0f;
		float x = std::floor(Numbers::Random(-bounds, bounds));
		float y = std::floor(Numbers::Random(-bounds, bounds));
		goodie->SetPosition(x, y);
		goodies.Add(goodie);
	}

	// update camera
/*	auto cam = GetCameraEntity();
	if(snakes.Size() == 0){
		cam->SetPosition(0, 0);
	} else {
		auto first = snakes.items[0].lock()->head.lock();
		float left = first->position.x;
		float right = first->position.x;
		float top = first->position.y;
		float bottom = first->position.y;
		for(auto& snake: snakes.items){
			auto cur = snake.lock()->head.lock();
			if(cur->position.x < left) left = cur->position.x;
			if(cur->position.x > right) right = cur->position.x;
			if(cur->position.y < bottom) bottom = cur->position.y;
			if(cur->position.y > top) top = cur->position.y;
		}
		float w = right - left + 8;
		float h = top - bottom + 8;
		float aspect = w/h;
		float center_x = (right+left)/2.0f;
		float center_y = (top+bottom)/2.0f;
		cam->SetPosition(center_x, center_y);
		if(aspect > Gfx::aspect){
			cam->SetScale(1/Gfx::width*w);
		} else {
			cam->SetScale(1/Gfx::height*h);
		}

	} */

	auto snake = snakes.items[0].lock();
	auto head = snake->head.lock();
	GetCameraEntity()->SetPosition(head->GetWorldPosition());
//	GetCameraEntity()->SetRotation(head->GetWorldAngle() - Numbers::Pi/2.0f);

}
void GameState::OnRender(){

}
void GameState::OnExit(){

}
