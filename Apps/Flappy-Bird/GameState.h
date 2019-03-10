#pragma once

#include "Ledlib2d/State.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Ledlib/Util/Timer.h"
#include <glm/vec2.hpp>
#include <vector>

using namespace Ledlib;

class BirdEntity;
class PipeEntity;

namespace Ledlib {
class Bitmap;
}

class GameState : public State {
public:
	const int STATE_INTRO = 0;
	const int STATE_PLAYING = 1;
	const int STATE_OUTRO = 2;
	ColorRgb backgroundColor;
	int state = STATE_INTRO;
	Timer outroTimer;
	glm::vec2 textOffset;
	int score;
	Timer scoreFlashTimer;
	Timer superScoreFlashTimer;
	std::vector<std::weak_ptr<BirdEntity>> birds;
	std::vector<std::weak_ptr<PipeEntity>> pipes;
	float nextPipePosition;
	float nextScorePosition;

	GameState();
	void OnStart();
	void OnEnd();
	void OnUpdate();
	void OnBeforeRender();
	void OnAfterRender();

	void UpdateScore();
	void HandleCollision(BirdEntity* bird, PipeEntity* pipe);
};
