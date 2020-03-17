#pragma once

#include <memory>

namespace Ledlib {

class Entity;

class Component {
public:
	std::weak_ptr<Entity> entity;
	Component();
};

}
