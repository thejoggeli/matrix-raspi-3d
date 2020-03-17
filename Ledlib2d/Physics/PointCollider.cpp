#include "PointCollider.h"

namespace Ledlib {

PointCollider::PointCollider(){}

void PointCollider::OnSetup(){
	type = ColliderType::Point;
	_boundingRadius = 0;
	_boundingRadiusSquared = 0;
}


}

