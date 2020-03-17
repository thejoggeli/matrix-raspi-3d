#include "ShaderState.h"
#include "Ledlib2d/Gfx/ShaderBox.h"

ShaderState::ShaderState(){}

void ShaderState::OnRender(){
	shader->Use();
	Gfx::DrawShaderBox(*shader.get(), 0, 0);
}
