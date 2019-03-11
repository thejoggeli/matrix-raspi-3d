#include "Tetris.h"
#include "GameState.h"
#include "Ledlib2d/Resources/ResourceManager.h"
#include "Ledlib/Sfx/RemoteSfx.h"

void Tetris::OnSetup(){

}

void Tetris::OnStart(){
	RemoteSfx::AddFile("a-theme", "tetris/a-theme.mp3");
	RemoteSfx::autoplayMusic = true;
	SetState<GameState>();
}
