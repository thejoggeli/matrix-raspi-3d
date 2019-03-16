#include "MenuState.h"
#include "GameState.h"
#include "Pong.h"
#include "Ledlib/Util/ClientJoiner.h"
#include "Ledlib/Util/ClientJoinerSlot.h"
#include "Ledlib2d/Resources/ResourceManager.h"
#include "Ledlib2d/Resources/Bitmap.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Text.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Gfx/Bitmaps.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Ledlib/Util/ColorHsl.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Time.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Remote/Client.h"

MenuState::MenuState(){}
MenuState::~MenuState(){}

void MenuState::OnStart(){
	ClientJoiner::Init(2, false);
	bgBitmap = Bitmap::CreateEmpty(Gfx::width, Gfx::height);
	bgBitmap->Clear(0, 0, 0, 0);
}

void MenuState::OnUpdate(){
	ClientJoiner::Update();
	if(ClientJoiner::IsEveryoneReady() && ClientJoiner::GetNumTakenSlots() > 0){
		if(startTimerRunning){
			if(startTimer.IsFinished()){
				std::shared_ptr<Pong> pong = GetGame<Pong>();
				// start pong
				std::shared_ptr<GameState> state = pong->SetState<GameState>();
				// pass clients
				for(auto slot: ClientJoiner::GetTakenSlots()){
					if(!slot->GetClient()) continue;
					state->AddStartClient(slot->GetClient());
				}
			}
		} else {
			startTimerRunning = true;
			startTimer.Start(1.5f);
			bgBitmap->Clear(0, 0, 0, 0);
		}
	} else if(startTimerRunning){
		startTimerRunning = false;
	}

}
void MenuState::OnRender(){
	if(startTimerRunning){
		float t = Time::deltaTime;
		while(t > 0.0f){
			t-=0.00025f;
			ColorHsl hsl = ColorHsl(Numbers::Wrap(0.f, 1.f, Numbers::Random(-0.2f, 0.2f)), 0.75f, 0.35f);
			bgBitmap->SetPixel(Numbers::Random(0, Gfx::width), Numbers::Random(0, Gfx::height), hsl.GetRgb());
		}
		bgBitmap->Update();
		Gfx::DrawBitmap(bgBitmap.get(), 0, 0);
	}
	// slots
	int i = 0;
	int slotWidth = 24;
	int slotHeight = 16;
	int boundsWidth = slotWidth + 6;
	int boundsHeight = slotHeight + 12;
	Gfx::SetFont(ResourceManager::GetFont("menu-2"));
	for(auto const& it: ClientJoiner::GetAllSlots()){
		std::shared_ptr<ClientJoinerSlot> slot = it;
		Gfx::SetDrawColor(ColorRgb::RED);
		int cx = boundsWidth*(i-1) + boundsWidth/2;
		int cy = -4;
		Gfx::StrokeRect(cx, cy, slotWidth, slotHeight);
		Gfx::textCharacterSpacing = 1;
		if(slot->IsTaken()){
			std::shared_ptr<Client> client = ClientManager::GetClient(slot->clientId);
			if(slot->IsReady()){
				Gfx::SetDrawColor(ColorRgb::GREEN);
			} else {
				Gfx::SetDrawColor(ColorRgb::LIGHT_GRAY);
			}
			Gfx::FillRect(cx, cy, slotWidth-2, slotHeight-2);
			if(client->IsKeyDown(KeyCode::A) || client->IsKeyDown(KeyCode::Start)){
				Gfx::SetTextColor(ColorRgb::RED);
			} else {
				Gfx::SetTextColor(ColorRgb::BLACK);
			}
			Gfx::DrawText("P"+std::to_string(i+1), cx, cy);
		} else {
			if(Numbers::Mod(Time::currentTime*0.5f, 1.0f) < 0.5f){
				Gfx::SetTextColor(ColorRgb::LIGHT_GRAY);
				Gfx::DrawText("?", cx, cy);
			} else {
				Gfx::SetTextColor(ColorRgb::LIGHT_GRAY);
				Gfx::DrawText("!", cx, cy);
			}
		}
		Gfx::textCharacterSpacing = 0;
		i++;
	}
	Gfx::SetFont(ResourceManager::GetFont("menu-1"));
	Gfx::textBaseline = TextBaseline::Middle;
	Gfx::textAlign = TextAlign::Center;
	if(startTimerRunning){
		Gfx::SetTextColor(ColorRgb::WHITE);
		Gfx::DrawText("GET READY", 0, Gfx::top-6);
	} else if(Numbers::Mod(Time::currentTime*0.5f, 1.0f) < 0.5f){
		Gfx::SetTextColor(ColorRgb::WHITE);
		Gfx::DrawText("JOIN NOW", 0, Gfx::top-6);
	} else {
		Gfx::SetTextColor(ColorRgb::WHITE);
		Gfx::DrawText("PRESS START", 0, Gfx::top-6);
	}
	if(startTimerRunning){
		ColorRgb color = ColorRgb::BLACK;
		color.a = Numbers::Clamp(0.0f, 1.0f, 1.0f * startTimer.GetRelativePassedTime());
		Gfx::SetDrawColor(color);
		Gfx::FillRect(0, 0, Gfx::width, Gfx::height);
	}
}
