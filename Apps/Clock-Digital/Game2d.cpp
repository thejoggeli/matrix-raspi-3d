#include "Game2d.h"
#include "State/GameState.h"
#include "Ledlib2d/Resources/ResourceManager.h"
#include "Ledlib/Config.h"

void Game2d::OnSetup(){
//	Config::Set("opengl_multisampling_enabled", "1", Config::Target::Runtime);
//	Config::Set("opengl_multisampling_samples", "2", Config::Target::Runtime);
	ResourceManager::LoadFontBdf("clock-0", "7x13B.bdf");
	ResourceManager::LoadFontBdf("clock-1", "7x13.bdf");
	ResourceManager::LoadFontBdf("clock-2", "tom-thumb.bdf");
	ResourceManager::LoadFontBdf("clock-3", "clR6x12.bdf");
	ResourceManager::LoadFontBdf("clock-4", "helvR12.bdf");
	SetState<GameState>();
}

void Game2d::OnStart(){
}

void Game2d::OnExit(){}
