#include "Game2d.h"
#include "State/MenuState.h"

void Game2d::OnSetup(){}

void Game2d::OnStart(){
	SetState<MenuState>();
}

void Game2d::OnEnd(){}
