#include "PolygonCollider.h"
#include "Ledlib/Log.h"
#include "../Gfx/Transform.h"
#include "../Gfx/Draw.h"
#include "../Gfx/Path.h"
#include "Ledlib/Math/Numbers.h"
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

namespace Ledlib {

PolygonCollider::PolygonCollider(){}

void PolygonCollider::OnSetup(){
	type = ColliderType::Polygon;
}
void PolygonCollider::OnUpdateTransform(){
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(_position, 0));
	transform = glm::rotate(transform, _rotation, glm::vec3(0,0,1));
	transform = glm::scale(transform, glm::vec3(_scale, 1.0));
	for(int t = 0; t < worldTriangles.size(); t++){
		worldTriangles[t][0] = transform * glm::vec4(triangles[t][0], 0.0f, 1.0f);
		worldTriangles[t][1] = transform * glm::vec4(triangles[t][1], 0.0f, 1.0f);
		worldTriangles[t][2] = transform * glm::vec4(triangles[t][2], 0.0f, 1.0f);
	}
	float maxSquared = 0;
	for(int i = 0; i < triangles.size(); i++){
		for(int t = 0; t < 3; t++){
			glm::vec2& v = triangles[i][t];
			float d = (v.x*v.x*_scale.x*_scale.x + v.y*v.y*_scale.y*_scale.y);
			if(d > maxSquared){
				maxSquared = d;
			}
		}
	}
	_boundingRadiusSquared = maxSquared;
	_boundingRadius = sqrtf(_boundingRadiusSquared);
}

void PolygonCollider::AddPoint(float x, float y){
	points.push_back(glm::vec2(x,y));
	SetNeedsUpdate();
}
void PolygonCollider::AddIndex(int i){
	indices.push_back(i);
	if(indices.size()%3==0){
		int i = indices.size()-3;
		std::vector<glm::vec2> t;
		t.push_back(points[indices[i]]);
		t.push_back(points[indices[i+1]]);
		t.push_back(points[indices[i+2]]);
		triangles.push_back(t);
		worldTriangles.push_back(t);
	}
	SetNeedsUpdate();
}
void PolygonCollider::AddTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3){
	int index = points.size();
	AddPoint(p1.x, p1.y);
	AddPoint(p2.x, p2.y);
	AddPoint(p3.x, p3.y);
	AddIndex(index);
	AddIndex(index+1);
	AddIndex(index+2);
	SetNeedsUpdate();
}

void PolygonCollider::SetBox(float w, float h){
	Clear();
	AddPoint(-0.5f*w, -0.5f*h);
	AddPoint(-0.5f*w, +0.5f*h);
	AddPoint(+0.5f*w, +0.5f*h);
	AddPoint(+0.5f*w, -0.5f*h);
	AddIndex(0);
	AddIndex(1);
	AddIndex(2);
	AddIndex(0);
	AddIndex(2);
	AddIndex(3);
	SetNeedsUpdate();
}
void PolygonCollider::SetCircle(float radius, int numPoints){
	if(numPoints < 3) numPoints = 3;
	AddPoint(0,0);
	float angleStep = Numbers::Pi*2.0f / static_cast<float>(numPoints);
	float angle = 0.0f;
	for(int i = 0; i < numPoints; i++){
		AddPoint(cosf(angle)*radius, sinf(angle) * radius);
		angle += angleStep;
	}
	for(int i = 1; i < numPoints; i++){
		AddIndex(0);
		AddIndex(i);
		AddIndex(i+1);
	}
	AddIndex(0);
	AddIndex(numPoints);
	AddIndex(1);
	SetNeedsUpdate();
}

void PolygonCollider::Clear(){
	points.clear();
	indices.clear();
	triangles.clear();
	worldTriangles.clear();
	SetNeedsUpdate();
}

void PolygonCollider::OnDebugDraw(){
	Gfx::SetDrawColor(1, 0.5f, 0.5f, 0.75f);
	for(int i = 0; i < worldTriangles.size(); i++){
		Gfx::BeginPath();
		Gfx::LineTo(worldTriangles[i][0].x, worldTriangles[i][0].y);
		Gfx::LineTo(worldTriangles[i][1].x, worldTriangles[i][1].y);
		Gfx::LineTo(worldTriangles[i][2].x, worldTriangles[i][2].y);
		Gfx::ClosePath();
		Gfx::FillPath();
	}
}


void PolygonCollider::GenerateTriangleStrip(){
	if(points.size() < 3){
		Log(LOG_ERROR, "PolygonCollider", "GenerateTriangleStrip() not enough points");
		return;
	}
	int iters = points.size()-2;
	for(int i = 0; i < iters; i++){
		AddIndex(i);
		AddIndex(i+1);
		AddIndex(i+2);
	}
	SetNeedsUpdate();
}
void PolygonCollider::GenerateTriangleFan(){
	if(points.size() < 3){
		Log(LOG_ERROR, "PolygonCollider", "GenerateTriangleFan() not enough points");
		return;
	}
	int iters = points.size()-1;
	for(int i = 1; i < iters; i++){
		AddIndex(0);
		AddIndex(i);
		AddIndex(i+1);
	}
	SetNeedsUpdate();
}

}
