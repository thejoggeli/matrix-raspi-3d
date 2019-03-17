#pragma once

#include "Ledlib2d/Entity.h"

using namespace Ledlib;

class Snake;

class SnakeSegment : public Entity {
public:
	int depth = 0;
	bool isHead = false;
	bool isLoose = false;
	float radius = 2.0f;
	float nextDistance = 7.0f;
	float despawnTimer = 1.0f;
	std::weak_ptr<Snake> snake;
	std::weak_ptr<SnakeSegment> next;
	void OnStart();
	void OnUpdate();
	void OnRender();
	void OnEnd();
	void SetSnake(const std::shared_ptr<Snake>& snake);
	void Grow();
	void Cut();
	void SetLoose();
};

