#include "Basic.h"
#include "ShaderPlayground.h"
#include "Ledlib2d/Gfx/ShaderBox.h"
#include "Ledlib2d/Gfx/Transform.h"
#include "Ledlib2d/Scene.h"
#include "Ledlib2d/Camera.h"
#include "Ledlib/Remote/ClientManager.h"

Basic::Basic(){}

void Basic::OnStart(){
	args = glm::vec4(1, 0, 0, 0);
	shader->AddArgs4f("args");
	shader->SetArgs4f("args", args);
}

void Basic::OnUpdate(){
	GetCamera()->SimpleJoystickMove();
	GetCamera()->SimpleArrowsZoom();
	GetCamera()->SimpleArrowsRotate();
	if(ClientManager::OnKeyDown(KeyCode::A)){
		args[0] += 0.25f;
		if(args[0] > 1.0f){
			args[0] = 0.25f;
		}
		shader->SetArgs4f("args", args);
	}
}

void Basic::OnRender(){
	Gfx::Save();
	GetCamera()->ApplyTransform();
	shader->Use();
	Gfx::DrawShaderBox(*shader.get(), 0, 0);
	Gfx::Restore();
}
