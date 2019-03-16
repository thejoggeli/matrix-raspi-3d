#include "Game2d.h"
#include "State/GameState.h"
#include "Ledlib2d/Resources/ResourceManager.h"
#include "Ledlib/Config.h"

void Game2d::OnSetup(){
//	Config::Set("opengl_multisampling_enabled", "1", Config::Target::Runtime);
//	Config::Set("opengl_multisampling_samples", "2", Config::Target::Runtime);
}

void Game2d::OnStart(){
	ResourceManager::LoadFontBdf("clock", "7x13B.bdf");
	SetState<GameState>();
}

void Game2d::OnEnd(){}
