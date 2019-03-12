#pragma once

#include "Ledlib2d/State.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Ledlib/Util/Timer.h"
#include <glm/vec2.hpp>
#include <vector>
#include "Ledlib2d/WeakPointerList.h"

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
	WeakPointerList<PipeEntity> pipes;
	WeakPointerList<BirdEntity> birds;
	float nextPipePosition;
	float nextScorePosition;
	float outroDirection;

	GameState();
	void OnStart();
	void OnEnd();
	void OnUpdate();
	void OnRender();
	void OnAfterRender();

	void UpdateScore();
	void HandleCollision(BirdEntity* bird, PipeEntity* pipe);
};

