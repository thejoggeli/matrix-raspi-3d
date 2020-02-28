#include "TestGame.h"
#include "MenuState.h"
#include "Ledlib2d/Resources/ResourceManager.h"

void TestGame::OnSetup(){
	ResourceManager::LoadFontBdf("font-1", "6x12.bdf");
	SetState<MenuState>();
}

void TestGame::OnStart(){

}
