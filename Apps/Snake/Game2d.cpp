#include "Game2d.h"
#include "State/GameState.h"
#include "Ledlib/Config.h"

void Game2d::OnSetup(){
//	Config::Set("opengl_multisampling_enabled", "1", Config::Target::Runtime);
//	Config::Set("opengl_multisampling_samples", "2", Config::Target::Runtime);
	SetState<GameState>();
}

void Game2d::OnStart(){
}

void Game2d::OnExit(){}
