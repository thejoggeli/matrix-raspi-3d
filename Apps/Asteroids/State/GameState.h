#pragma once

#include "Ledlib2d/State.h"
#include "Ledlib2d/WeakPointerList.h"

using namespace Ledlib;
using namespace std;

class Player;
class Asteroid;

class GameState : public State {
private:
public:

	WeakPointerList<Player> players;
	WeakPointerList<Asteroid> asteroids;

	shared_ptr<Asteroid> SpawnAsteroid();
	void Boundify(Entity& entity, float radius);

	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnEnd();

};
