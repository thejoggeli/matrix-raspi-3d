#include "GameState.h"
#include "MenuState.h"
#include "Piece.h"
#include "Grid.h"
#include "Tetris.h"
#include "TetrisPlayer.h"
#include "Ledlib2d/Resources/Bitmap.h"
#include "Ledlib2d/Entity.h"
#include "Ledlib2d/Camera.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Bitmaps.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Gfx/Transform.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Ledlib/Util/ColorHsl.h"
#include "Ledlib/Time.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Remote/Client.h"
#include "Ledlib/Sfx/RemoteSfx.h"
#include "Ledlib/Events/EventManager.h"

using namespace Ledlib;

GameState::GameState(){}
GameState::~GameState(){}

void GameState::OnStart(){
	bgBitmap = Bitmap::CreateEmpty(Gfx::width, Gfx::height);
	Gfx::SetClearColor(ColorRgb::BLACK);
	// play music
	RemoteSfx::StartMusic(0, "a-theme");
	for(auto& player: players){
		player->grid.Start();
	}
}

void GameState::OnExit(){
	RemoteSfx::StopMusic(0);
	for(auto& client: ClientManager::GetAllCients()){
		client->UnsetFlag(0);
	}
}

void GameState::OnUpdate(){
	std::shared_ptr<Entity> camera = GetCameraEntity();
	if(players.size() < 3){
		for(auto& client: ClientManager::GetAllCients()){
		//	if(client->IsFlagSet(0)) continue;
			if(client->OnKeyDown(KeyCode::A) || client->OnKeyDown(KeyCode::Start)){
				TetrisPlayer& player = AddPlayer(client);
				player.grid.Start();
			}
		}
	}
	for(auto& player: players){
		if(player->client.expired()) continue;
		player->grid.Update(player->client.lock().get());
	}
/*	if(ClientManager::OnKeyDown(KeyCode::Start)){
		GetGame()->SetState<GameState>();
		camera->SetPosition(0, 0);
		camera->SetRotation(0);
		camera->SetScale(1);
	} */
	GetCamera()->SimpleJoystickZoom();
	GetCamera()->SimpleJoystickRotate(-1.0f);
}

void GameState::OnRender(){
	// background
	bgHue = fmodf(bgHue + Time::deltaTime * 0.1f, 1.0f);
	bgAngle = fmodf(bgAngle + Time::deltaTime*0.5f, 3.1415926535897932384f*2.0f);
	ColorHsl color = ColorHsl(bgHue, 0.8f, 0.075f);
	float mag = 0.01f;
	float sin = sinf(bgAngle) * mag;
	float cos = cosf(bgAngle) * mag;
	for(int x = 0; x < Gfx::width; x++){
		for(int y = 0; y < Gfx::height; y++){
			color.h = fmodf(bgHue+x*sin+y*cos, 1.0f);
			bgBitmap->SetPixel(x, y, color.GetRgb());
		}
	}
	bgBitmap->Update();
	Gfx::DrawBitmap(bgBitmap.get(), 0.0f, 0.0f);
	// camera
	Gfx::Save();
	GetCamera()->ApplyTransform();
	// y-mirror
	Gfx::Save();
	Gfx::Scale(1, -1);
	// tetris grid
	Gfx::Save();
	Gfx::Scale(1, 1);
	int i = 0;
	for(auto& player: players){
		if(!player) continue;

		Gfx::Save();

		Gfx::Translate(-3.0f, 0.0f);
		if(i == 1){
			Gfx::Translate(-21.0f, 0.0f);
		} if(i == 2){
			Gfx::Translate(21.0f, 0.0f);
		}

		Grid& grid = player->grid;
		Gfx::Translate(-grid.width/2.0f, -grid.height/2.0f);

		Gfx::SetDrawColor(ColorRgb::BLACK);
		Gfx::FillRect(grid.width/2.0f, grid.height/2.0f, grid.width, grid.height);

		ColorHsl c = ColorHsl(Time::sinceStart*0.2f, 0.125f, 0.25f);
		grid.Render();
		Gfx::SetDrawColor(0.5f, 0, 0);
		Gfx::FillRect(-1+0.5f, grid.height/2.0f, 1, grid.height);
		Gfx::FillRect(grid.width+0.5f, grid.height/2.0f, 1, grid.height);

		// next piece
		Gfx::Translate(grid.width + 2.0f, 3.0f);
		Gfx::strokeAlign = StrokeAlign::Outside;
		Gfx::StrokeRect(2,2,6,6);
		Gfx::SetDrawColor(ColorRgb::BLACK);
		Gfx::FillRect(2,2,6,6);

		Piece& nextPiece = *grid.nextPiece;
		Gfx::SetDrawColor(nextPiece.color);
		for(int x = 0; x < nextPiece.size; x++){
			for(int y = 0; y < nextPiece.size; y++){
				if(!nextPiece.slots[x][y]) continue;
				Gfx::FillRect(0.5f+x, 0.5f+y, 1, 1);
			}
		}
		i++;
		Gfx::Restore();
	}
	Gfx::Restore();
	Gfx::Restore(); // y-mirror
	Gfx::Restore(); // camera

}

TetrisPlayer& GameState::AddPlayer(const std::shared_ptr<Client>& client){
	std::shared_ptr<TetrisPlayer> player = std::make_shared<TetrisPlayer>();
	client->SetFlag(0);
	player->grid.Setup(10, 32);
	player->client = client;
	players.push_back(player);
	return *player;
}
