#include "Game2d.h"
#include "State/MenuState.h"
#include "State/GameState.h"

void Game2d::OnSetup(){
	SetState<GameState>();
}

void Game2d::OnStart(){

}

void Game2d::OnExit(){}
