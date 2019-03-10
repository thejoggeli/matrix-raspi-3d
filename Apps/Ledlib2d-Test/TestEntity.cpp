#include "TestEntity.h"
#include "MenuState.h"

#include "Ledlib/Time.h"
#include "Ledlib/Log.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Gfx/Transform.h"
#include "Ledlib2d/Gfx/Text.h"
#include "Ledlib2d/Game.h"
#include "Ledlib2d/Resources/ResourceManager.h"
#include "Ledlib2d/Scene.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Ledlib/Util/ColorHsl.h"

void TestEntity::OnStart(){
	hue = Numbers::Random(0.0f, 1.0f);
}
void TestEntity::OnUpdate(){
//	Translate(Time::deltaTime*5.0f, 0);
//	angle += Time::deltaTime*Numbers::Pi*0.25f;
//	SetRotation(Numbers::Stepify(angle, Numbers::Pi*2.0f/16.0f));
/*	if(ClientManager::OnKeyDown(KeyCode::A)){
		Game::GetInstance()->SetState<MenuState>();
	}
	if(ClientManager::OnKeyDown(KeyCode::Left)){
		std::shared_ptr<Entity> child = GetScene()->CreateEntity<Entity>(shared_from_this());
	}
	if(ClientManager::OnKeyDown(KeyCode::Right)){
		for(auto& entity: GetChildren()){
			entity->Destroy();
		}
	} */
	if(ClientManager::OnKeyDown(KeyCode::Down)){
	//	Destroy();
	}
}
void TestEntity::OnRender(){
	ColorHsl hsl = ColorHsl(hue, 1.0f, 0.5f);
	Gfx::SetDrawColor(hsl.GetRgb());
	Gfx::FillRect(0, 0, 10, 10);
	Gfx::strokeAlign = StrokeAlign::Outside;
	Gfx::SetDrawColor(0, 0, 1);
	Gfx::StrokeRect(0, 0, 10, 10);
	Gfx::SetFont(ResourceManager::GetFont("font-1"));
	Gfx::SetTextColor(ColorRgb::WHITE);
	Gfx::SetTextPosition(TextAlign::Center, TextBaseline::Middle);
	Gfx::Translate(0,0,0.1f);
	Gfx::DrawText(std::to_string(GetChildren().size()), 0, 0);
	Gfx::Save();
	Gfx::Translate(0, 6, 0);
	Gfx::Translate(0,0,0.1f);
	Gfx::SetDrawColor(ColorRgb::CYAN);
	Gfx::FillRect(0,0,4,4);
	Gfx::Restore();
}

void TestEntity::OnCollision(const CollisionData& data){
	Log("Collision!");
	if(this->HasTag("bounce")){
		Translate(15, 0);
	/*	auto& children = GetChildren();
		for(auto& child: children){
			child->Scale(1.1f, 1.1f);
		} */
	} else {
		hue = Numbers::Random(0.0f, 1.0f);
	}
}
