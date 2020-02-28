#include "GameState.h"
#include "Game2d.h"
#include "Entity/Snake.h"
#include "Entity/SnakeSegment.h"
#include "Entity/Goodie.h"
#include "Entity/Wall.h"
#include "Ledlib2d/Scene.h"
#include "Ledlib/Math/Numbers.h"

void GameState::OnStart(){
//	GetScene()->SetDebugDrawEnabled(true);

	std::shared_ptr<Snake> snake;
	snake = GetScene()->CreateEntity<Snake>("snake");
	snakes.Add(snake);
	GetScene()->EnableCollision("snake-head", "snake-head");
	GetScene()->EnableCollision("snake-head", "snake-tail");
	GetScene()->EnableCollision("snake-head", "goodie");
	GetScene()->EnableCollision("snake-head", "wall");
	GetScene()->EnableCollision("snake-tail", "wall");

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
	auto snake = snakes.items[0].lock();
	auto head = snake->head.lock();
	while(goodies.Size() < 100){
		auto goodie = GetScene()->CreateEntity<Goodie>("goodie");
		float bounds = 128.0f;
		float x = std::floor(Numbers::Random(-bounds, bounds));
		float y = std::floor(Numbers::Random(-bounds, bounds));
		goodie->SetPosition(x, y);
		goodies.Add(goodie);
	}
	GetCameraEntity()->SetPosition(head->GetWorldPosition());
//	GetCameraEntity()->SetRotation(head->GetWorldAngle() - Numbers::Pi/2.0f);
}
void GameState::OnRender(){

}
void GameState::OnExit(){

}
