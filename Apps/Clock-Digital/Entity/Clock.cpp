#include "Clock.h"
#include "Ledlib2d/Gfx/Gfx.h"
#include "Ledlib2d/Gfx/Renderer.h"
#include "Ledlib2d/Gfx/Text.h"
#include "Ledlib2d/Gfx/Bitmaps.h"
#include "Ledlib2d/Gfx/Draw.h"
#include "Ledlib2d/Resources/ResourceManager.h"
#include <time.h>
#include "Ledlib2d/Resources/Bitmap.h"
#include "Ledlib2d/Gfx/ShaderBox.h"

Clock::Clock(){}

void Clock::OnStart(){
	bitmap = Bitmap::CreateRenderable(Gfx::width, Gfx::height);
	shader = std::make_shared<ShaderBox>();
	shader->LoadFile("clock/clock.frag");
	shader->AddArgsBitmap("iClockTex");
}
void Clock::OnUpdate(){}

void Clock::RenderToTexture(){
	bitmap->SetRenderTarget(true);
	Gfx::Clear(0, 0, 0, 0);
	// draw clock to bitmap
	static char timeBuffer[16];
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime (&rawtime);
	strftime(timeBuffer,sizeof(timeBuffer),"%H:%M:%S",timeinfo);
	std::string str = timeBuffer;
	Gfx::antiKerning = false;
	Gfx::SetTextPosition(TextAlign::Center, TextBaseline::Middle);
	Gfx::SetTextColor(1, 1, 1);
	Gfx::SetFont(ResourceManager::GetFont("clock"));
	Gfx::DrawText(str, 0, 0);
	bitmap->SetRenderTarget(false);
}

void Clock::OnRender(){
/*	Gfx::SetDrawColor(0,0,1);
	Gfx::FillRect(0, 0, 60, 16);
	Gfx::strokeAlign = StrokeAlign::Outside;
	Gfx::SetDrawColor(1,0,0);
	Gfx::StrokeRect(0, 0, 60, 16); */

//	Gfx::DrawBitmap(bitmap.get(), 0, 0);
	shader->SetArgsBitmap("iClockTex", bitmap.get(), 0);
	Gfx::DrawShaderBox(*shader.get(), 0, 0);

}
void Clock::OnEnd(){}
