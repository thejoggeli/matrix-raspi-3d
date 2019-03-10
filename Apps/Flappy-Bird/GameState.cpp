#include "GameState.h"
#include "MenuState.h"
#include "FlappyBird.h"
#include "PipeEntity.h"
#include "BirdEntity.h"
#include <algorithm>
#include "Ledlib/Sfx/RemoteSfx.h"
#include "Ledlib/Time.h"
#include "Ledlib/Log.h"
#include "Ledlib/Remote/Client.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Util/ColorHsl.h"
#include "Ledlib2d/Camera.h"
#include "Ledlib2d/Scene.h"
#include "Ledlib2d/Game.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Renderer.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Gfx/Text.h"
#include "Ledlib2d/Gfx/Bitmaps.h"
#include "Ledlib2d/Gfx/Transform.h"
#include "Ledlib2d/Resources/ResourceManager.h"
#include <algorithm>

GameState::GameState(){}

void GameState::OnStart(){
	RemoteSfx::PlaySound(0, "start");
	state = STATE_PLAYING;
	backgroundColor.SetRgb(0.1f, 0.2f, 0.3f);
	Gfx::SetClearColor(backgroundColor);
	GetCamera()->GetEntity()->SetPosition(0, 0);
	pipes.clear();

	GetScene()->EnableCollision("bird", "pipe");
	std::shared_ptr<BirdEntity> bird = GetScene()->CreateEntity<BirdEntity>();
	Bitmap* bitmap = GetGame<FlappyBird>()->birdBitmap;
	if(bitmap == nullptr) bitmap = ResourceManager::GetBitmap("bird-yellow");
	bird->bitmap = bitmap;
	birds.push_back(bird);

	nextPipePosition = Gfx::right+1;
	RemoteSfx::StartMusic(0, "music-game");
	nextScorePosition = nextPipePosition;
	scoreFlashTimer.Reset();
	superScoreFlashTimer.Reset();
	score = 0;

}
void GameState::OnEnd(){
	FlappyBird* game = GetGame<FlappyBird>().get();
	if(score > game->highscore){
		game->highscore = score;
	}
}
void GameState::OnUpdate(){
	std::shared_ptr<Entity> camera = GetCamera()->GetEntity();
	if(state == STATE_PLAYING){
		camera->Translate(Time::deltaTime * 20.0f, 0);
		while(camera->position.x+Gfx::right*2 > nextPipePosition){
			float x = nextPipePosition;
			int gap = Numbers::Random(9, 16);
			float center = Numbers::Random(-15, 15);
			int gap_top = gap%2 == 0 ? gap/2 : gap/2+1;
			int gap_bottom = gap/2;
			// pipe top
			std::shared_ptr<PipeEntity> pipeTop = GetScene()->CreateEntity<PipeEntity>();
			pipeTop->SetRotation(Numbers::Pi);
			pipeTop->SetPosition(x, center + gap_top + pipeTop->height/2);
			// pipe bottom
			std::shared_ptr<PipeEntity> pipeBottom = GetScene()->CreateEntity<PipeEntity>();
			pipeBottom->SetPosition(x, center - gap_bottom - pipeBottom->height/2);
			// invisible pipes
			float maxPos = Gfx::top+pipeTop->height/2-2;
			if(pipeTop->position.y > maxPos){
				pipeTop->Destroy();
			} else {
				pipes.push_back(pipeTop);
			}
			float minPos = Gfx::bottom-pipeBottom->height/2+2;
			if(pipeBottom->position.y < minPos){
				pipeBottom->Destroy();
			} else {
				pipes.push_back(pipeBottom);
			}
			// next position
			nextPipePosition += Numbers::Random(20, 28);
		}
		for(auto const& pipe: pipes){
			auto p = pipe.lock();
			if(p && p->position.x+p->width + 2 < camera->position.x + Gfx::left){
				p->Destroy();
			}
		}
		UpdateScore();
		for(auto bird = birds.begin(); bird < birds.end(); bird++){
			if(bird->expired()){
				birds.erase(bird);
				bird--;
			}
		}
		for(auto pipe = pipes.begin(); pipe < pipes.end(); pipe++){
			if(pipe->expired()){
				pipes.erase(pipe);
				pipe--;
			}
		}
		if(birds.size() == 0){
			state = STATE_OUTRO;
		}
	} else if(state == STATE_OUTRO){
		if(ClientManager::OnKeyDown(KeyCode::A)){
			GetGame()->SetState<GameState>();
		}
		textOffset = glm::vec2(0, 0);
		for(auto& client: ClientManager::GetAllCients()){
			if(client->IsKeyDown(KeyCode::LeftJoystick)){
				Vector2f joy = client->GetJoystickPosition(KeyCode::LeftJoystick);
				textOffset = glm::vec2(joy.x, joy.y);
				textOffset *= 5;
				break;
			}
		}
	}
	if(ClientManager::OnKeyDown(KeyCode::Start)){
		GetGame()->SetState<MenuState>();
		RemoteSfx::PlaySound(0, "exit");
	}
}
void GameState::OnBeforeRender(){
	std::shared_ptr<Entity> camera = GetCamera()->GetEntity();
	Gfx::SetBitmapColor(0.75f, 0.75f, 0.75f);
	float clouds_offset = -Numbers::Mod(camera->position.x*0.6f, 64.0f);
	for(float i = 0; i < Gfx::width*2; i += 64.0f){
		Gfx::DrawBitmap(ResourceManager::GetBitmap("bg-clouds"), i+clouds_offset, Gfx::bottom+6);
	}
	float grass_offset = -Numbers::Mod(camera->position.x*0.8f, 64.0f);
	for(float i = 0; i < Gfx::width*2; i += 64.0f){
		Gfx::DrawBitmap(ResourceManager::GetBitmap("bg-grass"), i+grass_offset, Gfx::bottom+3);
	}
	Gfx::SetBitmapColor(1,1,1);
}

void GameState::OnAfterRender(){
	std::shared_ptr<Entity> camera = GetCamera()->GetEntity();
	// game over
	if(state == STATE_OUTRO){
		ColorRgb color = backgroundColor;
		color.a = 0.5f;
		Gfx::SetDrawColor(color);
		Gfx::FillRect(0, 0, Gfx::width, Gfx::height);
		// game over font
		Gfx::Save();
		Gfx::Translate(textOffset.x*1.25f, textOffset.y);
		if(textOffset.x != 0 || textOffset.y != 0){
			float y = Numbers::Clamp(0.25f, 1.0f, textOffset.y);
			float angle = atan2f(y, textOffset.x*0.025f);
			Gfx::Rotate(angle - Numbers::Pi/2);
		}
		Gfx::SetTextColor(1, 1, 1);
		Gfx::SetFont(ResourceManager::GetFont("menu-large"));
		Gfx::textBaseline = TextBaseline::Bottom;
		Gfx::textAlign = TextAlign::Center;
		Gfx::DrawText("GAME", 0, -1);
		Gfx::textBaseline = TextBaseline::Top;
		Gfx::DrawText("OVER", 0, -1);
		Gfx::Restore();
	}
	if(superScoreFlashTimer.IsRunning()){
		ColorHsl hsl = ColorHsl(0, 1.0f, 0.65f);
		hsl.h = hsl.WrapHue(Time::sinceStart*5.0f);
		Gfx::SetTextColor(hsl.GetRgb());
	} else {
		Gfx::SetTextColor(scoreFlashTimer.IsFinished() ? ColorRgb::WHITE : ColorRgb::YELLOW);
	}
	// score
	Gfx::SetFont(ResourceManager::GetFont("menu-small"));
	Gfx::SetTextPosition(TextAlign::Left, TextBaseline::Top);
	Gfx::DrawText(std::to_string(score), Gfx::left+1, Gfx::top-1);
}

void  GameState::UpdateScore(){
	for(auto &bird: birds){
		if(auto p = bird.lock()){
			if(p->position.x > nextScorePosition){
				score++;
				if(score%10==0){
					superScoreFlashTimer.Start(0.40f);
				} else {
					scoreFlashTimer.Start(0.10f);
				}
				for(auto const& pipe: pipes){
					if(auto p = pipe.lock()){
						if(p->position.x > nextScorePosition){
							nextScorePosition = p->position.x;
							return;
						}
					}
				}
			}
		}
	}
}
