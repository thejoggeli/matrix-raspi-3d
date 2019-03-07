#include "TestEntity.h"
#include "MenuState.h"

#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Game.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Ledlib/Util/ColorHsl.h"

void TestEntity::OnStart(){
	hue = Numbers::Random(0.0f, 1.0f);
}
void TestEntity::OnUpdate(){
	if(ClientManager::OnKeyDown(KeyCode::A)){
		Game::GetInstance()->SetState<MenuState>();
	}
}
void TestEntity::OnRender(){
	ColorHsl hsl = ColorHsl(hue, 1.0f, 0.5f);
	Gfx::SetDrawColor(hsl.GetRgb());
	Gfx::FillRect(0, 0, 10, 10);
	Gfx::strokeAlign = StrokeAlign::OUTSIDE;
	Gfx::SetDrawColor(0, 0, 1);
	Gfx::StrokeRect(0, 0, 10, 10);
}
