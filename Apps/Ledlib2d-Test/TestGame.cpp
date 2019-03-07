#include "TestGame.h"
#include "MenuState.h"

void TestGame::OnSetup(){

}

void TestGame::OnStart(){
	SetState<MenuState>();
}
