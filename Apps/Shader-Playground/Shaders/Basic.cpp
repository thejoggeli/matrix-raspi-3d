#include "Basic.h"
#include "ShaderPlayground.h"
#include "Ledlib2d/Gfx/ShaderBox.h"
#include "Ledlib2d/Scene.h"
#include "Ledlib2d/Camera.h"

Basic::Basic(){}

void Basic::OnStart(){
	// nothing
}

void Basic::OnUpdate(){
	GetCamera()->SimpleJoystickMove();
	GetCamera()->SimpleArrowsZoom();
	GetCamera()->SimpleArrowsRotate();
}

void Basic::OnRender(){
	shader->Use();
	shader->SetCameraPosition(GetCameraEntity()->GetWorldPosition());
	shader->SetCameraRotation(GetCameraEntity()->GetWorldRotation());
	shader->SetCameraScale(GetCameraEntity()->GetWorldScale());
	Gfx::DrawShaderBox(*shader.get(), 0, 0);
}
