#include "Asteroid.h"
#include "Game2d.h"
#include "State/GameState.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Time.h"
#include "Ledlib/Log.h"
#include "Ledlib/Util/ColorHsl.h"
#include "Ledlib2d/Physics/PolygonCollider.h"
#include "Ledlib2d/Physics/CollisionData.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Gfx/Path.h"
#include "Ledlib2d/Scene.h"
#include <cmath>

Asteroid::Asteroid(){
	velocity = glm::vec3(Numbers::Random(-5.0f, 5.0f), Numbers::Random(-5.0f, 5.0f), 0);
	angularVelocity = Numbers::Random(-1.0f, 1.0f);
	hue = Numbers::Random(0.0f, 1.0f);
}
void Asteroid::Generate(float minRadius, float maxRadius, int minPoints, int maxPoints){
	std::shared_ptr<PolygonCollider> collider = Collider::Create<PolygonCollider>("asteroid");
	int numPoints = Numbers::Random(minPoints, maxPoints+1);
	float step = M_PIf32 * 2.0f / numPoints;
	for(int i = 0; i < numPoints; i++){
		float radius = Numbers::Random(minRadius, maxRadius);
		glm::vec2 point = glm::vec2(std::cos(i*step) * radius, std::sin(i*step) * radius);
		collider->AddPoint(point.x, point.y);
		vertices.push_back(point);
	}
	collider->GenerateTriangleFan();
	collider->drawBoundingCircle = true;
	SetCollider(collider);
	ColorHsl hsl(hue, 1.0f, 0.5f);
	color = hsl.GetRgb();
}
void Asteroid::OnStart(){}
void Asteroid::OnUpdate(){
	Translate(velocity * Time::deltaTime);
	Rotate(angularVelocity * Time::deltaTime);
	// bounds
	Game::GetInstance()->GetState<GameState>()->Boundify(*this, GetCollider()->_boundingRadius);
}
void Asteroid::OnRender(){
	Gfx::lineWidth = 0.75f;
	Gfx::SetDrawColor(color);
	Gfx::BeginPath();
	for(int i = 0; i < vertices.size(); i++){
		Gfx::LineTo(vertices[i].x, vertices[i].y);
	}
	Gfx::ClosePath();
	Gfx::StrokePath();
}
void Asteroid::OnEnd(){}
void Asteroid::OnCollision(const CollisionData& data){
	if(data.b->group == "player"){
		float max = GetCollider()->_boundingRadius * 0.8f;
		float min = max*0.4f;
		if(max > 3.0f){
			std::shared_ptr<Asteroid> asteroid;
			asteroid = Game::GetInstance()->GetState<GameState>()->SpawnAsteroid();
			asteroid->hue = Numbers::Wrap(0.0f, 1.0f, hue - 0.02f);
			asteroid->Generate(min, max, 3, 10);
			asteroid->SetPosition(position);
			asteroid->angularVelocity = angularVelocity * 1.25f;
			asteroid = Game::GetInstance()->GetState<GameState>()->SpawnAsteroid();
			asteroid->hue = Numbers::Wrap(0.0f, 1.0f, hue + 0.02f);
			asteroid->Generate(min, max, 4, 10);
			asteroid->SetPosition(position);
			asteroid->angularVelocity = angularVelocity * 0.75f;
		}
		Destroy();
	}
}
