#include "Renderer.h"
#include "Gfx.h"
#include "Transform.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Ledlib/Display/DisplayManager.h"
#include "Ledlib/Log.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"
#include "../Scene/Entity.h"
#include <GLES2/gl2.h>

namespace Ledlib {
namespace Gfx {

static ColorRgb _clearColor;
static uint8_t* pixels;
static bool _autoClear = true;

void InitRenderer(int width, int height){
	// display
	int pitch = 3;
	pixels = (uint8_t*) malloc(width * height * sizeof(uint8_t) * pitch);
	DisplayManager::SetPixelsPointer(pixels, pitch);

}

void SetAutoClear(bool autoClear){
	_autoClear = autoClear;
}

void Render(Scene* scene, Camera* camera){
	if(_autoClear){
		Clear();
	}
	Save();
	Transform(glm::inverse(camera->GetEntity()->GetWorldMatrix()));
	std::vector<Entity*>& entities = scene->GetEntities();
	for(auto& entity: entities){
		Save();
		const glm::mat4& modelMatrix = entity->GetWorldMatrix();
		Transform(modelMatrix);
		entity->OnRender();
		Restore();
	}
	Restore();
}

void UpdatePixelBuffer(){
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
}

// clear
void Clear(){
	glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void SetDepthTestEnabled(bool e){
	if(e){
		glEnable(GL_DEPTH_TEST);
	} else {
		glDisable(GL_DEPTH_TEST);
	}
}
void SetClearColor(float r, float g, float b, float a){
	_clearColor.r = r; _clearColor.g = g; _clearColor.b = b; _clearColor.a = a;
}
void SetClearColor(const ColorRgb& color){
	_clearColor = color;
}

}
}
