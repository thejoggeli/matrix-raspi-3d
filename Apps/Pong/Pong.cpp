#include "FlappyBird.h"
#include "MenuState.h"
#include "Ledlib2d/Resources/ResourceManager.h"

void FlappyBird::OnSetup(){

}

void FlappyBird::OnStart(){
	SetState<MenuState>();
}
