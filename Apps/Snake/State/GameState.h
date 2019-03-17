#pragma once

#include "Ledlib2d/State.h"
#include "Ledlib2d/WeakPointerList.h"

class Snake;
class Goodie;
class Wall;

using namespace Ledlib;

class GameState : public State {
private:
public:
	WeakPointerList<Snake> snakes;
	WeakPointerList<Goodie> goodies;
	WeakPointerList<Wall> walls;
	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnEnd();

};
