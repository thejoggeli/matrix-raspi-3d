#include "CollisionManager.h"
#include "CollisionData.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "CircleCollider.h"
#include "PolygonCollider.h"
#include "PointCollider.h"
#include "../Entity.h"
#include <math.h>
#include "Ledlib/Log.h"

namespace Ledlib {

static inline float Det2D(glm::vec2 &p1, glm::vec2 &p2, glm::vec2 &p3);
static void CheckTriWinding(glm::vec2 &p1, glm::vec2 &p2, glm::vec2 &p3, bool allowReversed);
static bool BoundaryCollideChk(glm::vec2 &p1, glm::vec2 &p2, glm::vec2 &p3, float eps);
static bool BoundaryDoesntCollideChk(glm::vec2 &p1, glm::vec2 &p2, glm::vec2 &p3, float eps);
static bool TriTri2D(glm::vec2 *t1, glm::vec2 *t2, float eps = 0.0f, bool allowReversed = false, bool onBoundary = true);

CollisionManager::CollisionManager(){}

bool CollisionManager::CheckCollision(Collider* _a, Collider* _b){
	Collider* a;
	Collider* b;
	if(_a->type < _b->type){
		a = _a;
		b = _b;
	} else {
		a = _b;
		b = _a;
	}
	if(a->needsUpdate){
		a->UpdateTransform();
	}
	if(b->needsUpdate){
		b->UpdateTransform();
	}
	if(a->type == ColliderType::Point && b->type == ColliderType::Point){
		PointCollider* p1 = static_cast<PointCollider*>(a);
		PointCollider* p2 = static_cast<PointCollider*>(b);
		if(p1->_position == p2->_position){
			PropagateCollison(CollisionData(p1, p2), CollisionData(p2, p1));
			return true;
		}
		return false;
	} else if(a->type == ColliderType::Point && b->type == ColliderType::Circle){
		PointCollider* p1 = static_cast<PointCollider*>(a);
		CircleCollider* p2 = static_cast<CircleCollider*>(b);
		glm::vec2 delta = p1->_position - p2->_position;
		float distance = sqrtf(delta.x*delta.x + delta.y*delta.y);
		if(distance < p2->_boundingRadius){
			PropagateCollison(CollisionData(p1, p2), CollisionData(p2, p1));
			return true;
		}
		return false;
	} else if(a->type == ColliderType::Circle && b->type == ColliderType::Circle){
		CircleCollider* p1 = static_cast<CircleCollider*>(a);
		CircleCollider* p2 = static_cast<CircleCollider*>(b);
		glm::vec2 delta = p1->_position - p2->_position;
		float distance = sqrtf(delta.x*delta.x + delta.y*delta.y);
		if(distance < p1->_boundingRadius + p2->_boundingRadius){
			PropagateCollison(CollisionData(p1, p2), CollisionData(p2, p1));
			return true;
		}
		return false;
	} else if(a->type == ColliderType::Point && b->type == ColliderType::Box){
		PointCollider* p1 = static_cast<PointCollider*>(a);
		BoxCollider* p2 = static_cast<BoxCollider*>(b);
		if(p1->_position.x < p2->worldLeft) return false;
		if(p1->_position.x > p2->worldRight) return false;
		if(p1->_position.y < p2->worldBottom) return false;
		if(p1->_position.y > p2->worldTop) return false;
		PropagateCollison(CollisionData(p1, p2), CollisionData(p2, p1));
		return true;
	} else if(a->type == ColliderType::Box && b->type == ColliderType::Box){
		BoxCollider* p1 = static_cast<BoxCollider*>(a);
		BoxCollider* p2 = static_cast<BoxCollider*>(b);
		if(p1->worldRight < p2->worldLeft) return false;
		if(p1->worldLeft > p2->worldRight) return false;
		if(p1->worldTop < p2->worldBottom) return false;
		if(p1->worldBottom > p2->worldTop) return false;
		PropagateCollison(CollisionData(p1, p2), CollisionData(p2, p1));
		return true;
	} else if(a->type == ColliderType::Polygon && b->type == ColliderType::Polygon){
		PolygonCollider* p1 = static_cast<PolygonCollider*>(a);
		PolygonCollider* p2 = static_cast<PolygonCollider*>(b);
		// bounds check
		glm::vec2 delta = p1->_position - p2->_position;
		float distance = sqrt(delta.x*delta.x + delta.y*delta.y);
		if(distance < (p1->_boundingRadius + p2->_boundingRadius)){
			// loop through triangles
			for(int i = 0; i < p1->worldTriangles.size(); i++){
				// loop through triangles
				for(int j = 0; j < p2->worldTriangles.size(); j++){
					// do triangles intersect?
					if(TriTri2D(p1->worldTriangles[i].data(), p2->worldTriangles[j].data(), 0.0, true)){
						PropagateCollison(CollisionData(p1, p2), CollisionData(p2, p1));
						return true;
					}
				}
			}
		}
		return false;
	}
	return false;
}

void CollisionManager::PropagateCollison(const CollisionData& c1, const CollisionData& c2){
	c1.a->GetEntity()->Collision(c1);
	c2.a->GetEntity()->Collision(c2);
}

static inline float Det2D(glm::vec2 &p1, glm::vec2 &p2, glm::vec2 &p3){
	return +p1.x*(p2.y-p3.y)
		+p2.x*(p3.y-p1.y)
		+p3.x*(p1.y-p2.y);
}

static void CheckTriWinding(glm::vec2 &p1, glm::vec2 &p2, glm::vec2 &p3, bool allowReversed){
	float detTri = Det2D(p1, p2, p3);
	if(detTri < 0.0f){
		if (allowReversed){
			glm::vec2 a = p3;
			p3 = p2;
			p2 = a;
		}
		else throw std::runtime_error("triangle has wrong winding direction");
	}
}

static bool BoundaryCollideChk(glm::vec2 &p1, glm::vec2 &p2, glm::vec2 &p3, float eps){
	return Det2D(p1, p2, p3) < eps;
}

static bool BoundaryDoesntCollideChk(glm::vec2 &p1, glm::vec2 &p2, glm::vec2 &p3, float eps){
	return Det2D(p1, p2, p3) <= eps;
}

static bool TriTri2D(glm::vec2 *t1, glm::vec2 *t2, float eps, bool allowReversed, bool onBoundary){
	//Trangles must be expressed anti-clockwise
	CheckTriWinding(t1[0], t1[1], t1[2], allowReversed);
	CheckTriWinding(t2[0], t2[1], t2[2], allowReversed);

	bool (*chkEdge)(glm::vec2 &, glm::vec2 &, glm::vec2 &, float) = nullptr;
	if(onBoundary) //Points on the boundary are considered as colliding
		chkEdge = BoundaryCollideChk;
	else //Points on the boundary are not considered as colliding
		chkEdge = BoundaryDoesntCollideChk;

	//For edge E of trangle 1,
	for(int i=0; i<3; i++){
		int j=(i+1)%3;
		//Check all points of trangle 2 lay on the external side of the edge E. If
		//they do, the triangles do not collide.
		if (chkEdge(t1[i], t1[j], t2[0], eps) &&
			chkEdge(t1[i], t1[j], t2[1], eps) &&
			chkEdge(t1[i], t1[j], t2[2], eps))
			return false;
	}

	//For edge E of trangle 2,
	for(int i=0; i<3; i++){
		int j=(i+1)%3;

		//Check all points of trangle 1 lay on the external side of the edge E. If
		//they do, the triangles do not collide.
		if (chkEdge(t2[i], t2[j], t1[0], eps) &&
			chkEdge(t2[i], t2[j], t1[1], eps) &&
			chkEdge(t2[i], t2[j], t1[2], eps))
			return false;
	}

	//The triangles collide
	return true;
}



}
