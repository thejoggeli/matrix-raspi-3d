#pragma once

#include "Ledlib2d/State.h"

using namespace Ledlib;

class MenuState : public State {
private:
public:
	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnBeforeRender();
	virtual void OnRender();
};
