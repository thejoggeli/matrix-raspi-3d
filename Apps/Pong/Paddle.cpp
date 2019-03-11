#include "Paddle.h"
#include "Pong.h"
#include "GameState.h"
#include "Ball.h"
#include "Ledlib2d/Resources/ResourceManager.h"
#include "Ledlib2d/Physics/BoxCollider.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Gfx/Text.h"
#include "Ledlib2d/Gfx/Transform.h"
#include "Ledlib/Remote/Client.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Ledlib/Time.h"
#include "Ledlib/Log.h"

Paddle::Paddle(){}

void Paddle::OnStart(){
	std::shared_ptr<BoxCollider> collider = Collider::Create<BoxCollider>("paddle");
	SetCollider(collider);
	collider->SetSize(width, height);
}

void Paddle::OnUpdate(){
	oldPosition = position;
	float maxVelocity = 50.0f;
	if(auto clientPtr = client.lock()){
		if(clientPtr->OnKeyDown(KeyCode::B)){
			this->client.reset();
			clientPtr->UnsetFlag(1);
		} else {
			// player controls
			if(clientPtr->IsKeyDown(KeyCode::LeftJoystick)){
				float jy = clientPtr->GetJoystickPosition(KeyCode::LeftJoystick).y;
				velocity.y = Numbers::Clamp(-maxVelocity, maxVelocity, maxVelocity * jy);
			} else {
				if(clientPtr->IsKeyDown(KeyCode::Up)){
					velocity.y = maxVelocity;
				} else if(clientPtr->IsKeyDown(KeyCode::Down)){
					velocity.y = -maxVelocity;
				} else {
					velocity.y = 0;
				}
			}
			Translate(0, Time::deltaTime * velocity.y);
		}
	} else {
		std::shared_ptr<Ball> ball = Game::GetInstance()->GetState<GameState>()->GetBall();
		// ai
		if(ball->position.y > position.y){
			velocity.y = maxVelocity;
		} else if(ball->position.y < position.y){
			velocity.y = -maxVelocity;
		} else {
			velocity.y = 0;
		}
		float dist = fabsf(position.x-ball->position.x);
		velocity *= Numbers::Clamp(0.1f, 1.0f, 1-dist/(64.0f*0.75)+0.25f);
		Translate(0, Time::deltaTime * velocity.y);
	}
	if(position.y + height/2 > Gfx::top-1){
		SetPosition(position.x, Gfx::top-1 - height/2);
	} else if(position.y - height/2 < Gfx::bottom+1){
		SetPosition(position.x, Gfx::bottom+1+height/2);
	}
}

void Paddle::OnRender(){
	// draw name
	Gfx::SetFont(ResourceManager::GetFont("ai"));
	Gfx::textBaseline = TextBaseline::Top;
	Gfx::SetTextColor(ColorRgb::GRAY);
	Gfx::Save();
	Gfx::ClearTransform();
	if(position.x > 0){
		Gfx::textAlign = TextAlign::Right;
		Gfx::DrawText(client.lock() ? name : "AI", Gfx::right-1, Gfx::top-2);
	} else {
		Gfx::textAlign = TextAlign::Left;
		Gfx::DrawText(client.lock() ? name : "AI", Gfx::left+1, Gfx::top-2);
	}
	Gfx::Restore();

	// draw paddle
	if(client.expired()){
		Gfx::SetDrawColor(ColorRgb::CYAN);
	} else {
		Gfx::SetDrawColor(ColorRgb::WHITE);
	}
	Gfx::FillRect(0, 0, width, height);
}

void Paddle::SetClient(std::shared_ptr<Client> client){
	if(client->IsFlagSet(1)){
		return;
	} else {
		this->client = client;
		client->SetFlag(1);
	}
}
