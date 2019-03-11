#pragma once

#include <memory>

namespace Ledlib {

class Collider;
class CollisionData;

class CollisionManager {
private:
	CollisionManager();
public:

	static bool CheckCollision(Collider* _a, Collider* _b);
	static void PropagateCollison(const CollisionData& c1, const CollisionData& c2);

};

}
