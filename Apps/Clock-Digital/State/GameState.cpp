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

	ex1 = Numbers::Random(0.75f, 0.125f);
	ey1 = Numbers::Random(0.75f, 0.125f);
	ez1 = Numbers::Random(0.2f, 0.4f);
	ex11 = Numbers::Random(0.8f, 1.2f);
	ey11 = Numbers::Random(0.4f, 0.6f);
	ez11 = Numbers::Random(0.4f, 0.6f);
	ex2 = Numbers::Random(0.6f, 1.0f);
	ey2 = Numbers::Random(0.1f, 0.125f);
	ez2 = Numbers::Random(0.04f, 0.06f);
	ex22 = Numbers::Random(0.2f, 0.3f);
	ey22 = Numbers::Random(0.008f, 0.012f);
	ez22 = Numbers::Random(0.008f, 0.012f);

}
void GameState::OnUpdate(){
	std::shared_ptr<Entity> camera = GetCameraEntity();
	glm::vec3 eulers;
	eulers.y = sinf(Time::sinceStart*ex1)*ex11;
	eulers.x = cosf(Time::sinceStart*ey1)*ey11;
	eulers.z = sinf(Time::sinceStart*ez1)*ez11;
	clock->SetScale(0.17f + sinf(Time::sinceStart)*0.07f);
	clock->SetRotation(eulers);

	eulers = glm::vec3(0.0f);
	eulers.y = cosf(Time::sinceStart*ex2)*ex22;
	eulers.x = sinf(Time::sinceStart*ey2)*ey22;
	eulers.z = cosf(Time::sinceStart*ez2)*ez22;
	camera->SetRotation(eulers);
//	camera->SetPosition(sinf(Time::sinceStart*1.0f)*5.0f, cosf(Time::sinceStart*0.084323f)*0.2f, camera->position.z);

}
void GameState::OnRender(){
	clock->RenderToTexture();

}
void GameState::OnAfterRender(){
	Gfx::DrawShaderBox(*border.get(), 0, 0);
}

void GameState::OnExit(){
	Config::Set("app-offset-hours", std::to_string(clock->offsetHours), Config::Target::App);
}
