#pragma once

#include <memory>

namespace Ledlib {

class Collider;

struct CollisionData {
	Collider* a;
	Collider* b;
	CollisionData(Collider* a, Collider*b);
};

}


