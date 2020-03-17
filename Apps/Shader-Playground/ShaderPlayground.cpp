#include "ShaderPlayground.h"
#include "Shaders/Basic.h"
#include "Shaders/Mandelbrot.h"
#include "Ledlib2d/Gfx/ShaderBox.h"

ShaderPlayground::ShaderPlayground(){}

void ShaderPlayground::OnSetup(){
	std::shared_ptr<ShaderState> state;
	if(file == "playground/mandelbrot.frag"){
		state = SetState<Mandelbrot>();
	} else {
		state = SetState<Basic>();
	}
	state->shader = std::make_unique<ShaderBox>();
	state->shader->LoadFile(file.c_str());
}

void ShaderPlayground::OnStart(){
}
