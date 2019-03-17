#include "Wall.h"
#include "Ledlib2d/Physics/PolygonCollider.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Gfx/Path.h"

void Wall::OnStart(){}
void Wall::OnUpdate(){}
void Wall::OnRender(){
	Gfx::SetDrawColor(1,1,1);
	Gfx::BeginPath();
	for(auto& vertex: vertices){
		Gfx::LineTo(vertex.x, vertex.y);
	}
	Gfx::ClosePath();
	Gfx::StrokePath();
}
void Wall::OnEnd(){}

void Wall::AddVertex(float x, float y){
	vertices.push_back(glm::vec3(x, y, 0));
}
void Wall::Build(){
	auto collider = Collider::Create<PolygonCollider>("wall");
	for(auto& vertex: vertices){
		collider->AddPoint(vertex.x, vertex.y);
	}
	collider->GenerateTriangleFan();
	SetCollider(collider);

}
