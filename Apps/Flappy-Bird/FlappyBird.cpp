#include "FlappyBird.h"
#include "MenuState.h"
#include "Ledlib2d/Resources/ResourceManager.h"
#include "Ledlib/Sfx/RemoteSfx.h"
#include "Ledlib/Config.h"
#include "Ledlib/Log.h"

void FlappyBird::OnSetup(){
	highscore = Config::GetInt("app-highscore");
	Log(LOG_INFO, "FlappyBird", iLog << "Highscore is " << highscore);
	ResourceManager::LoadBitmapPng("bird-yellow", "flappy-bird/bird-yellow.png");
	ResourceManager::LoadBitmapPng("bird-red", "flappy-bird/bird-red.png");
	ResourceManager::LoadBitmapPng("bird-green", "flappy-bird/bird-green.png");
	ResourceManager::LoadBitmapPng("pipe", "flappy-bird/pipe.png");
	ResourceManager::LoadBitmapPng("bg-grass", "flappy-bird/bg-grass.png");
	ResourceManager::LoadBitmapPng("bg-clouds", "flappy-bird/bg-clouds.png");
	ResourceManager::LoadBitmapPng("selection", "flappy-bird/selection.png");
	ResourceManager::LoadFontBdf("menu-large", "7x13B.bdf");
	ResourceManager::LoadFontBdf("menu-small", "5x8.bdf");
	RemoteSfx::AddFile("jump", "fx217.mp3");
	RemoteSfx::AddFile("exit", "chuckle.mp3");
	RemoteSfx::AddFile("music-game", "music/funky-chiptune.mp3");
	RemoteSfx::AddFile("music-menu", "music/the-8-bit-digger.mp3");
	RemoteSfx::AddFile("die", "smb1/bowser-die.mp3", 1);
	RemoteSfx::AddFile("bump", "smb1/bump.mp3", 1);
	RemoteSfx::AddFile("start", "smb1/coin.mp3", 0.7f);
	RemoteSfx::AddFile("score", "flappy-bird/score.mp3", 0.6f);
	RemoteSfx::AddFile("score-super", "flappy-bird/score.mp3", 0.6f);
	RemoteSfx::autoplayMusic = true;
	SetState<MenuState>();
}

void FlappyBird::OnStart(){
}

void FlappyBird::OnExit(){
	Config::Set("app-highscore", highscore, Config::Target::App);
}
