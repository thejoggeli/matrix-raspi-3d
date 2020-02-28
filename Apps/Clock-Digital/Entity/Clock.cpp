#include "Clock.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Renderer.h"
#include "Ledlib2d/Gfx/Text.h"
#include "Ledlib2d/Gfx/Bitmaps.h"
#include "Ledlib2d/Gfx/Transform.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Resources/ResourceManager.h"
#include <time.h>
#include "Ledlib2d/Resources/Bitmap.h"
#include "Ledlib2d/Gfx/ShaderBox.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Time.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Log.h"

Clock::Clock(){}

void Clock::OnStart(){
	bitmap = Bitmap::CreateRenderable(Gfx::width, Gfx::height);
	shader = std::make_shared<ShaderBox>();
	shader->LoadFile("clock/clock.frag");
	shader->AddArgsBitmap("iClockTex");
	shader->AddArgs4f("iMods");
	font = ResourceManager::GetFont("clock-"+std::to_string(fontId));
	ResetEffectTime();
}
void Clock::OnUpdate(){
	if(ClientManager::OnKeyDown(KeyCode::Left)){
		if(--fontId < 0) fontId = 4;
		font = ResourceManager::GetFont("clock-"+std::to_string(fontId));
	}
	if(ClientManager::OnKeyDown(KeyCode::Right)){
		if(++fontId > 4) fontId = 0;
		font = ResourceManager::GetFont("clock-"+std::to_string(fontId));
	}
	if(ClientManager::IsKeyDown(KeyCode::Up)){
		extraScale.x += extraScale.x*Time::deltaTime;
		extraScale.y += extraScale.x*Time::deltaTime;
	}
	if(ClientManager::IsKeyDown(KeyCode::Down)){
		extraScale.x -= extraScale.x*Time::deltaTime;
		extraScale.y -= extraScale.x*Time::deltaTime;
	}
	if(ClientManager::OnKeyDown(KeyCode::Start)){
		offsetHours += 1;
	}
	if(ClientManager::OnKeyDown(KeyCode::Select)){
		offsetHours -= 1;
	}
	if(Time::sinceStart > nextEffectTime){
	/*	int sign = 1; // Numbers::Random(0, 2) == 0 ? 1 : -1;
		if(Numbers::Random(0, 2) == 0){
			spin += static_cast<float>(Numbers::Random(1, 6) * sign);
		} else {
			impulse += static_cast<float>(Numbers::Random(1, 6) * sign);
		} */
		spin += static_cast<float>(Numbers::Random(1, 6));
		ResetEffectTime();
	}
	if(ClientManager::OnKeyDown(KeyCode::A)){
		impulse += 2.0f;
		ResetEffectTime();
	}
	if(impulse > 0.0f){
		impulse -= Time::deltaTime*2.5f;
		if(impulse < 0.0f) impulse = 0.0f;
	}
	if(ClientManager::OnKeyDown(KeyCode::B)){
		spin += 1.0f;
		ResetEffectTime();
	}
	if(spin > 0.0f){
		spin -= Time::deltaTime*1.25f;
		if(spin < 0.0f) spin = 0.0f;
	}
}

void Clock::RenderToTexture(){
	Gfx::Save();
	bitmap->SetRenderTarget(true);
	Gfx::Clear(0, 0, 0, 0);
	// draw clock to bitmap
	if(fontId == 2){
		Gfx::Scale(1.4f, 1.4f);
	}
	static char timeBuffer[16];
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	rawtime += offsetHours * 3600;
	timeinfo = localtime (&rawtime);
	strftime(timeBuffer,sizeof(timeBuffer),"%H:%M:%S",timeinfo);
	std::string str = timeBuffer;
	Gfx::antiKerning = false;
	Gfx::SetTextPosition(TextAlign::Center, TextBaseline::Middle);
	Gfx::SetTextColor(1, 1, 1);
	Gfx::SetFont(font);
	Gfx::DrawText(str, 0, 0);
	bitmap->SetRenderTarget(false);
	Gfx::Restore();
}

void Clock::OnRender(){
/*	Gfx::SetDrawColor(0,0,1);
	Gfx::FillRect(0, 0, 60, 16);
	Gfx::strokeAlign = StrokeAlign::Outside;
	Gfx::SetDrawColor(1,0,0);
	Gfx::StrokeRect(0, 0, 60, 16); */
	Gfx::Rotate(spin*3.141f*2.0f, glm::vec3(1, 0, 0));
	Gfx::Scale(extraScale.x, extraScale.y ,extraScale.x);
//	Gfx::DrawBitmap(bitmap.get(), 0, 0);
	shader->SetArgs4f("iMods", glm::vec4(impulse, 0, 0, 0));
	shader->SetArgsBitmap("iClockTex", bitmap.get(), 0);
	Gfx::DrawShaderBox(*shader.get(), 0, 0);

}
void Clock::OnExit(){}

void Clock::ResetEffectTime(){
	nextEffectTime = Time::sinceStart + Numbers::Random(15.0f, 60.0f);
}
