#pragma once

#include "Ledlib2d/Entity.h"

using namespace Ledlib;

namespace Ledlib {
class Client;
}

class Paddle : public Entity {
public:

	std::string playerName;
	std::weak_ptr<Client> client;
	glm::vec2 oldPosition;
	glm::vec2 velocity;
	float width = 1;
	float height = 4;

	Paddle();

	void OnStart();
	void OnUpdate();
	void OnRender();
	void SetClient(std::shared_ptr<Client> client);
};

