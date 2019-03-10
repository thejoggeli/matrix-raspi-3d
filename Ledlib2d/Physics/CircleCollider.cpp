#include "CircleCollider.h"

namespace Ledlib {

CircleCollider::CircleCollider(){}

void CircleCollider::OnSetup(){
	type = ColliderType::Circle;
}

void CircleCollider::OnUpdateTransform(){
	worldRadius = (_scale.x + _scale.y) / 2.0f;
	worldRadiusSquared = worldRadius * worldRadius;
	_boundingRadius = worldRadius;
	_boundingRadius = worldRadiusSquared;
}

void CircleCollider::SetRadius(float r){
	radius = r;
	radiusSquared = r*r;
	SetNeedsUpdate();
}

}

