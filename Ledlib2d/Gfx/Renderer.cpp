#include "Renderer.h"
#include "Gfx.h"
#include "Transform.h"
#include "Ledlib/Util/ColorRgb.h"
#include "Ledlib/Display/DisplayManager.h"
#include "Ledlib/Log.h"
#include "../Scene.h"
#include "../Camera.h"
#include "../Entity.h"
#include <GLES2/gl2.h>
#include "../Physics/Collider.h"
#include "../Resources/Bitmap.h"

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

void RenderPrepare(){
	if(_autoClear){
		Clear();
	}
}
void Render(Scene* scene, Camera* camera){
	glm::mat4 oldProjectionMatrix = Gfx::projectionMatrix;
	Save();
	SetProjectionMatrix(camera->projectionMatrix);
	Transform(glm::inverse(camera->GetEntity()->GetWorldMatrix()));
	std::vector<Entity*>& entities = scene->GetEntities();
	for(auto& entity: entities){
		Save();
		const glm::mat4& modelMatrix = entity->GetWorldMatrix();
		Transform(modelMatrix);
		entity->OnRender();
		Restore();
	}
	if(scene->debugDrawEnabled){
		for(auto& entity: entities){
			std::shared_ptr<Collider> collider = entity->GetCollider();
			if(collider){
				Save();
				if(collider->needsUpdate){
					collider->UpdateTransform();
				}
				collider->DebugDraw();
				Restore();
			}
		}
	}
	Restore();
	Gfx::SetProjectionMatrix(oldProjectionMatrix);
}

void UpdatePixelBuffer(){
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
}

// clear
void Clear(){
	glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Clear(float r, float g, float b, float a){
	glClearColor(r, g, b, a);
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

void SetRenderTarget(const Bitmap* bitmap){
	if(bitmap == nullptr){
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Gfx::width, Gfx::height);
	} else {
		glBindFramebuffer(GL_FRAMEBUFFER, bitmap->framebufferId);
		glViewport(0, 0, bitmap->width, bitmap->height);
	}
}

}
}
