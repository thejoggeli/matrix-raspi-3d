#include "BoxCollider.h"
#include "../Gfx/Draw.h"
#include "Ledlib/Log.h"
#include <math.h>

namespace Ledlib {

BoxCollider::BoxCollider(){}

void BoxCollider::OnSetup(){
	type = ColliderType::Box;
}

void BoxCollider::OnUpdateTransform(){
	worldWidth = width * _scale.x;
	worldHeight = height * _scale.y;
	worldLeft = _position.x - worldWidth/2.0f;
	worldRight = _position.x + worldWidth/2.0f;
	worldBottom = _position.y - worldHeight/2.0f;
	worldTop = _position.y + worldHeight/2.0f;
	_boundingRadiusSquared = worldWidth*worldWidth/4.0f + worldHeight*worldHeight/4.0f;
	_boundingRadius = sqrtf(_boundingRadiusSquared);
}
void BoxCollider::OnDebugDraw(){
	Gfx::SetDrawColor(1,0,0, 0.75f);
	Gfx::FillRect(_position.x, _position.y, worldWidth, worldHeight);
//	Gfx::SetDrawColor(1,0,1, 1.0f);
/*	Gfx::lineWidth = 1.0f;
	Gfx::strokeAlign = StrokeAlign::Inside;
	Gfx::StrokeRect(_position.x, _position.y, worldWidth, worldHeight); */
}

void BoxCollider::SetSize(float w, float h){
	width = w;
	height = h;
	SetNeedsUpdate();
}

}

