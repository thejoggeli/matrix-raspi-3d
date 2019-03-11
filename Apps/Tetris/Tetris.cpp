#include "Tetris.h"
#include "GameState.h"
#include "Ledlib2d/Resources/ResourceManager.h"

void Tetris::OnSetup(){

}

void Tetris::OnStart(){
	SetState<GameState>();
}
