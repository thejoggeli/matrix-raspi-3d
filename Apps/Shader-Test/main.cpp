#include <iostream>

#include "Ledlib2d/Game.h"
#include "Ledlib2d/State.h"
#include "Ledlib2d/Gfx/Renderer.h"
#include "Ledlib2d/Gfx/ShaderBox.h"
#include "Ledlib2d/Gfx/Transform.h"
#include "Ledlib2d/Entity.h"
#include "Ledlib2d/Camera.h"

using namespace Ledlib;

class ShaderState : public State {
	ShaderBox shader;
	void OnStart(){
		Gfx::SetClearColor(0,0,0.3f);
		shader.LoadFile("torus_thingy_8.frag");
	}
	void OnUpdate(){
		GetCamera()->SimpleJoystickMove();
		GetCamera()->SimpleArrowsZoom();
		GetCamera()->SimpleArrowsRotate();
	}
	void OnRender(){
		shader.Use();
		shader.SetCameraPosition(GetCameraEntity()->GetWorldPosition());
		shader.SetCameraRotation(GetCameraEntity()->GetWorldRotation());
		shader.SetCameraScale(GetCameraEntity()->GetWorldScale());
		Gfx::Save();
	//	GetCamera()->ApplyTransform();
		Gfx::DrawShaderBox(shader, 0, 0);
		Gfx::Restore();
	}
};

class ShaderTest : public Game {
	void OnStart(){
		SetState<ShaderState>();
	}
};

int main(){
	ShaderTest::Run(new ShaderTest());
	return EXIT_SUCCESS;
}
