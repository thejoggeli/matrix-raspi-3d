#include "GameState.h"
#include "MenuState.h"
#include "Pong.h"
#include "Paddle.h"
#include "Ball.h"
#include "Ledlib/Log.h"
#include "Ledlib/Time.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Ledlib/Remote/Client.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib2d/Scene.h"
#include "Ledlib2d/Resources/Bitmap.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Gfx/Bitmaps.h"

GameState::GameState(){}
GameState::~GameState(){}

void GameState::OnStart(){
	bgBitmap = Bitmap::CreateEmpty(Gfx::width, Gfx::height-2);

	std::shared_ptr<Paddle> paddle;
	paddle = GetScene()->CreateEntity<Paddle>("paddle-1");
	paddle->name = "P1";
	paddle->SetPosition(Gfx::left + 1.5f, 0);
	paddles.Add(paddle);

	paddle = GetScene()->CreateEntity<Paddle>("paddle-2");
	paddle->name = "P2";
	paddle->SetPosition(Gfx::right - 1.5f, 0);
	paddles.Add(paddle);

	std::shared_ptr<Ball> ball;
	ball = GetScene()->CreateEntity<Ball>("ball-1");
	balls.Add(ball);

	beatTimer.Start(0.5f);
	borderColor = ColorHsl(0, 1, 0.5f);
	int i = 0;
	for(auto client: startClients){
		if(client.expired()) continue;
		client.lock()->UnsetFlag(1);
		paddles.items[i++].lock()->SetClient(client.lock());
	}


	for(auto p: balls.items) p.lock()->SetUpdateEnabled(false);
	introTimer.Start(1.5f);

	GetScene()->EnableCollision("paddle", "ball");
//	GetScene()->SetDebugDrawEnabled(true);

}
void GameState::OnUpdate(){
	if(!playing && introTimer.IsFinished()){
		for(auto p: balls.items) p.lock()->SetUpdateEnabled(true);
		playing = true;
	}
	if(!playing) return;
	for(auto& client: ClientManager::GetAllCients()){
		if(client->IsFlagSet(1)){
			if(client->OnKeyDown(KeyCode::Select)){
				GetGame()->SetState<MenuState>();
			}
		} else {
			if((client->OnKeyDown(KeyCode::Start) || client->OnKeyDown(KeyCode::A))){
				Log(LOG_DEBUG, "PongState", "Client wants to join");
				AssignClientToFreePaddle(client);
			}
		}
	}
	paddles.RemoveExpired();
	balls.RemoveExpired();
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
		for(int y = 0; y < Gfx::height-2; y++){
			color.h = fmodf(bgHue+x*sin+y*cos, 1.0f);
			bgBitmap->SetPixel(x, y, color.GetRgb());
		}
	}
	bgBitmap->Update();
	Gfx::DrawBitmap(bgBitmap.get(), 0.0f, 0.0f);
	// border
	borderColor.AddHue(Time::deltaTime*0.02f);
/*	borderColor.l += Time::deltaTime * 0.2f;
	borderColor.s -= Time::deltaTime * 0.2f;
	if(beatTimer.IsFinished()){
		beatTimer.Restart(true);
		borderColor.AddHue(0.333f);
		borderColor.l = 0.5;
		borderColor.s = 1.0f;
	} */
	Gfx::SetDrawColor(borderColor.GetRgb());
	Gfx::StrokeLine(Gfx::left, Gfx::bottom+0.5f, Gfx::right, Gfx::bottom+0.5f);
	Gfx::StrokeLine(Gfx::left, Gfx::top-0.5f, Gfx::right, Gfx::top-0.5f);

}
void GameState::OnAfterRender(){
	if(introTimer.IsRunning()){
		Gfx::SetDrawColor(0,0,0, Numbers::Clamp(0.f, 1.f, 1-introTimer.GetRelativePassedTime()*1.5f));
		Gfx::FillRect(0, 0, Gfx::width, Gfx::height);
	}
}

void GameState::AssignClientToFreePaddle(std::shared_ptr<Client> client){
	paddles.RemoveExpired();
	for(auto& paddle: paddles.items){
		if(paddle.lock()->client.expired()){
			paddle.lock()->SetClient(client);
			return;
		}
	}
}
void GameState::AddStartClient(std::shared_ptr<Client> client){
	startClients.push_back(client);
}
std::shared_ptr<Ball> GameState::GetBall(){
	balls.RemoveExpired();
	return balls.items[0].lock();
}
