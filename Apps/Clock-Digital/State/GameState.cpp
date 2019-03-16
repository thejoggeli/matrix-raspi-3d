#include "GameState.h"
#include "Ledlib2d/Scene.h"
#include "Ledlib2d/Camera.h"
#include "Entity/Clock.h"
#include "Ledlib/Time.h"
#include "Ledlib/Util/ColorHsl.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/ShaderBox.h"
#include <cmath>
#include "Ledlib/Config.h"

using namespace std;

void GameState::OnStart(){
	clock = GetScene()->CreateEntity<Clock>("clock");
	border = std::make_shared<ShaderBox>();
	border->LoadFile("clock/border.frag");
	GetCamera()->SetPerspective(50);
	GetCameraEntity()->SetPosition(0, 0, 12);
	clock->offsetHours = Config::GetInt("app-offset-hours", 0);
}
void GameState::OnUpdate(){
	std::shared_ptr<Entity> camera = GetCameraEntity();
	glm::vec3 eulers;
	eulers.y = sinf(Time::sinceStart*0.94323f);
	eulers.x = cosf(Time::sinceStart*1.12423f)*0.5f;
	eulers.z = cosf(Time::sinceStart*0.34323f)*0.5f;
	clock->SetScale(0.17f + sinf(Time::sinceStart)*0.07f);
	clock->SetRotation(eulers);

	eulers = glm::vec3(0.0f);
	eulers.y = sinf(Time::sinceStart*0.83423f) * 0.25f;
	eulers.x = sinf(Time::sinceStart*0.12321f) * 0.01f;
	eulers.z = sinf(Time::sinceStart*0.05342f) * 0.01f;
	camera->SetRotation(eulers);
//	camera->SetPosition(sinf(Time::sinceStart*1.0f)*5.0f, cosf(Time::sinceStart*0.084323f)*0.2f, camera->position.z);

}
void GameState::OnRender(){
	clock->RenderToTexture();

}
void GameState::OnAfterRender(){
	Gfx::DrawShaderBox(*border.get(), 0, 0);
}

void GameState::OnEnd(){
	Config::Set("app-offset-hours", std::to_string(clock->offsetHours), Config::Target::App);
}
