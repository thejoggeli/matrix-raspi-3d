#pragma once

#include "Ledlib2d/State.h"

class MenuState : public Ledlib::State {
private:
public:
	virtual void OnStart();
	virtual void OnUpdate();
	virtual void OnRender();
};
