#include "MenuState.h"
#include "GameState.h"
#include "FlappyBird.h"
#include "Ledlib/Sfx/RemoteSfx.h"
#include "Ledlib/Time.h"
#include "Ledlib/Log.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Remote/Client.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Renderer.h"
#include "Ledlib2d/Gfx/Text.h"
#include "Ledlib2d/Gfx/Bitmaps.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Gfx/Transform.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Ledlib/Util/ColorHsl.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib2d/Resources/ResourceManager.h"
#include "Ledlib2d/Camera.h"
#include "Ledlib2d/Entity.h"


void MenuState::OnStart(){
	RemoteSfx::StartMusic(0, "music-menu");
	Gfx::SetClearColor(0, 0, 0);
	flashTimer = 0;
	showHighscore = false;
}
void MenuState::OnExit(){
	RemoteSfx::StopMusic(0);
	GetGame<FlappyBird>()->birdBitmap = GetBirdBitmap();
}
void MenuState::OnUpdate(){
	showHighscore = ClientManager::IsKeyDown(KeyCode::A);
	if(ClientManager::OnKeyDown(KeyCode::Start)){
		std::shared_ptr<GameState> state = GetGame()->SetState<GameState>();
	}
	if(ClientManager::OnKeyDown(KeyCode::Select) || ClientManager::OnKeyDown(KeyCode::Right)){
		selection = (selection+1)%3;
		RemoteSfx::PlaySound(0, "jump");
	}
	if(ClientManager::OnKeyDown(KeyCode::Left)){
		selection = selection-1;
		if(selection < 0) selection = 2;
		RemoteSfx::PlaySound(0, "jump");
	}
	if(flashTimer > 0){
		flashTimer -= Time::deltaTime*2.0f;
	}
	if(ClientManager::OnKeyDown(KeyCode::B)){
		flashTimer = 1.2f;
		GetCamera()->GetEntity()->SetPosition(0, 0, 0);
		GetCamera()->GetEntity()->SetRotation(0);
		GetCamera()->GetEntity()->SetScale(1, 1, 1);
	}
	GetCamera()->SimpleJoystickZoom();
	GetCamera()->SimpleJoystickRotate(-1.0f);
}
void MenuState::OnRender(){
	Gfx::Save();
	GetCamera()->ApplyTransform();
	Gfx::SetDrawColor(0.1f, 0.2f, 0.3f);
	Gfx::FillRect(0, 0, Gfx::width, Gfx::height);
	if(flashTimer > 0){
		float alpha = Numbers::Clamp(0.0f, 1.0f, flashTimer*1.0f);
		Gfx::SetDrawColor(1.0f, 0, 0, alpha);
		Gfx::FillRect(0, 0, Gfx::width, Gfx::height);
	}
	Gfx::SetBitmapColor(0.5f, 0.5f, 0.5f);
	Gfx::DrawBitmap(ResourceManager::GetBitmap("bg-clouds"), 0, Gfx::bottom+6);
	Gfx::DrawBitmap(ResourceManager::GetBitmap("bg-grass"), 0, Gfx::bottom+3);
	Gfx::SetFont(ResourceManager::GetFont("menu-large"));
	Gfx::textBaseline = TextBaseline::Bottom;
	Gfx::textAlign = TextAlign::Center;
	Gfx::SetTextColor(ColorRgb::YELLOW);
	std::string text = Numbers::Mod(Time::sinceStart, 2.0f) < 1.0f ? "FLAPPY" : "BIRD";
	Gfx::DrawText(text, 0, 2);
	Gfx::SetTextColor(ColorRgb::WHITE);
	Gfx::textBaseline = TextBaseline::Top;
	Gfx::SetFont(ResourceManager::GetFont("menu-small"));
	Gfx::DrawText("PRESS START", 0, -7.5f);

	float h = -2;
	float left = Gfx::left/2;
	float middle = 0;
	float right = Gfx::right/2;
	Gfx::SetBitmapColor(1, 1, 1);
	Gfx::DrawBitmap(ResourceManager::GetBitmap("bird-green"), left+1, h);
	Gfx::DrawBitmap(ResourceManager::GetBitmap("bird-yellow"), middle+1, h);
	Gfx::DrawBitmap(ResourceManager::GetBitmap("bird-red"), right+1, h);

	float selectionPos;
	switch(selection){
	case 0: selectionPos = left; break;
	case 2: selectionPos = right; break;
	default: selectionPos = middle;
	}
	ColorHsl hsl = ColorHsl(0, 1.0f, 0.75f);
	hsl.h = hsl.WrapHue(Time::sinceStart*5.0f);
	Gfx::SetBitmapColor(hsl.GetRgb());
	Gfx::DrawBitmap(ResourceManager::GetBitmap("selection"), selectionPos, h+0.5f);
	Gfx::SetBitmapColor(1,1,1);
	Gfx::SetDrawColor(ColorRgb::RED);
	Gfx::StrokeRect(0, 0, Gfx::width, Gfx::height);

	if(showHighscore){
		int boxw = 48;
		int boxh = 18;
		Gfx::SetDrawColor(ColorRgb::BLACK);
		Gfx::FillRect(0, 0, boxw, boxh);
		Gfx::SetDrawColor(ColorRgb::RED);
		Gfx::StrokeRect(0, 0, boxw+2, boxh+2);
		Gfx::SetFont(ResourceManager::GetFont("menu-small"));
		Gfx::SetTextPosition(TextAlign::Center, TextBaseline::Middle);
		Gfx::SetTextColor(ColorRgb::WHITE);
		Gfx::DrawText("HIGHSCORE", 0, +4);
		int highscore = GetGame<FlappyBird>()->highscore;
		Gfx::DrawText(std::to_string(highscore), 0, -4);
	}
	Gfx::Restore();
}
Bitmap* MenuState::GetBirdBitmap(){
	switch(selection){
	case 0: return ResourceManager::GetBitmap("bird-green");
	case 2: return ResourceManager::GetBitmap("bird-red");
	}
	return ResourceManager::GetBitmap("bird-yellow");
}
