#pragma once

#include "Ledlib2d/State.h"
#include <vector>

using namespace Ledlib;

namespace Ledlib {
class Entity;
}

class MenuState : public State {
private:
	std::vector<std::weak_ptr<Entity>> entities;
public:
	virtual void OnStart();
	virtual void OnUpdate();
};
