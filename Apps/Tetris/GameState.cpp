#include "GameState.h"
#include "MenuState.h"
#include "Piece.h"
#include "Grid.h"
#include "Tetris.h"
#include "Ledlib2d/Entity.h"
#include "Ledlib2d/Camera.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Renderer.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Gfx/Transform.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Ledlib/Util/ColorHsl.h"
#include "Ledlib/Time.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Remote/Client.h"
#include "Ledlib/Sfx/RemoteSfx.h"

GameState::GameState(){}

void GameState::OnStart(){
	Gfx::SetClearColor(ColorRgb::BLACK);
	// play music
	RemoteSfx::StartMusic(0, "a-theme");

	// build grid
	grid = std::make_shared<Grid>();
	grid->Setup(10, 16);
	grid->Start();}

void GameState::OnEnd(){
	RemoteSfx::StopMusic(0);
}

void GameState::OnUpdate(){
	std::shared_ptr<Entity> camera = GetCameraEntity();
	grid->Update();
	if(ClientManager::OnKeyDown(KeyCode::Start)){
		GetGame()->SetState<GameState>();
		camera->SetPosition(0, 0);
		camera->SetRotation(0);
		camera->SetScale(1);
	}
	GetCamera()->SimpleJoystickZoom();
	GetCamera()->SimpleJoystickRotate(-1.0f);
}

void GameState::OnBeforeRender(){
	// camera
	Gfx::Save();
	GetCamera()->ApplyTransform();
	// y-mirror
	Gfx::Save();
	Gfx::Scale(1, -1);
	// tetris grid
	Gfx::Save();
	Gfx::Scale(2, 2);
	Gfx::Translate(-grid->width/2.0f, -grid->height/2.0f);

	ColorHsl c = ColorHsl(Time::sinceStart*0.2f, 0.125f, 0.25f);
	for(int i = 0; i < grid->width; i++){
		c.AddHue(0.2f);
		Gfx::SetDrawColor(c.GetRgb());
		Gfx::FillRect(0.5f + i, grid->height/2.0f, 1, grid->height);
	}
	grid->Render();
	Gfx::SetDrawColor(0.5f, 0, 0);
	Gfx::FillRect(-1+0.5f, grid->height/2.0f, 1, grid->height);
	Gfx::FillRect(grid->width+0.5f, grid->height/2.0f, 1, grid->height);

	// next piece
	Gfx::Translate(grid->width + 4.0f, 3.0f);
	Gfx::strokeAlign = StrokeAlign::Outside;
	Gfx::StrokeRect(2,2,6,6);
	Gfx::SetDrawColor(c.GetRgb());
	Gfx::FillRect(2,2,6,6);
	Piece& nextPiece = *grid->nextPiece;
	Gfx::SetDrawColor(nextPiece.color);
	for(int x = 0; x < nextPiece.size; x++){
		for(int y = 0; y < nextPiece.size; y++){
			if(!nextPiece.slots[x][y]) continue;
			Gfx::FillRect(0.5f+x, 0.5f+y, 1, 1);
		}
	}
	Gfx::Restore();
	Gfx::Restore(); // y-mirror
	Gfx::Restore(); // camera
}
