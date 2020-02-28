#include "Pong.h"
#include "MenuState.h"
#include "GameState.h"
#include "Ledlib2d/Resources/ResourceManager.h"

void Pong::OnSetup(){
	ResourceManager::LoadFontBdf("menu-1", "5x8.bdf");
	ResourceManager::LoadFontBdf("menu-2", "7x13.bdf");
	ResourceManager::LoadFontBdf("ai", "4x6.bdf");
	SetState<MenuState>();
}

void Pong::OnStart(){
}
