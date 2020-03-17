#pragma once

#include "Collider.h"

namespace Ledlib {

class PointCollider : public Collider {
public:
	PointCollider();

	virtual void OnSetup();

};

}

