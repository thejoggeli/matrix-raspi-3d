#include "GameState.h"
#include "Ledlib2d/Scene.h"
#include "Entity/Player.h"
#include "Entity/Asteroid.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib/Math/Numbers.h"

void GameState::OnStart(){
	shared_ptr<Player> player = GetScene()->CreateEntity<Player>("player");
	players.Add(player);
	player->Rotate(1.0f);
	shared_ptr<Asteroid> asteroid;
	GetScene()->SetDebugDrawEnabled(false);
	GetScene()->EnableCollision("player", "asteroid");
	GetScene()->EnableCollision("bullet", "asteroid");
}
void GameState::OnUpdate(){
	players.RemoveExpired();
	asteroids.RemoveExpired();
	while(asteroids.Size() < 3){
		float max = 12.0f;
		shared_ptr<Asteroid> asteroid;
		asteroid = SpawnAsteroid();
		asteroid->SetPosition(
			Numbers::Random(0, 2) == 0 ? Gfx::left - max : Gfx::right + max,
			Numbers::Random(0, 2) == 0 ? Gfx::bottom - max : Gfx::top + max
		);
		asteroid->Generate(6.0f, max, 4, 10);
	}
}
void GameState::OnRender(){}
void GameState::OnExit(){}

shared_ptr<Asteroid> GameState::SpawnAsteroid(){
	shared_ptr<Asteroid> asteroid = GetScene()->CreateEntity<Asteroid>("asteroid");
	asteroids.Add(asteroid);
	return asteroid;
}

void GameState::Boundify(Entity& entity, float radius){
	float left = Gfx::left - radius;
	float right = Gfx::right + radius;
	float bottom = Gfx::bottom - radius;
	float top = Gfx::top + radius;
	if(entity.position.x < left){
		entity.SetPosition(right, entity.position.y);
	}
	if(entity.position.x > right){
		entity.SetPosition(left, entity.position.y);
	}
	if(entity.position.y < bottom){
		entity.SetPosition(entity.position.x, top);
	}
	if(entity.position.y > top){
		entity.SetPosition(entity.position.x, bottom);
	}
}
