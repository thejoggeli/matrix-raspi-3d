#include "Mandelbrot.h"
#include "ShaderPlayground.h"
#include "Ledlib2d/Gfx/ShaderBox.h"
#include "Ledlib2d/Scene.h"
#include "Ledlib2d/Camera.h"

Mandelbrot::Mandelbrot(){}

void Mandelbrot::OnStart(){
	// nothing
}

void Mandelbrot::OnUpdate(){
	GetCamera()->SimpleJoystickMove();
	GetCamera()->SimpleArrowsZoom();
	GetCamera()->SimpleArrowsRotate();
}

void Mandelbrot::OnRender(){
	shader->Use();
	shader->SetCameraPosition(GetCameraEntity()->GetWorldPosition());
	shader->SetCameraRotation(GetCameraEntity()->GetWorldRotation());
	shader->SetCameraScale(GetCameraEntity()->GetWorldScale());
	Gfx::DrawShaderBox(*shader.get(), 0, 0);
}
