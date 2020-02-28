#include "Renderer.h"
#include "Gfx.h"
#include "Transform.h"
#include "Ledlib/Log.h"
#include "../Scene.h"
#include "../Camera.h"
#include "../Entity.h"
#include <GLES2/gl2.h>
#include "../Physics/Collider.h"

namespace Ledlib {
namespace Gfx {

static bool _autoClear = true;

void InitRenderer(int width, int height){

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

void SetDepthTestEnabled(bool e){
	if(e){
		glEnable(GL_DEPTH_TEST);
	} else {
		glDisable(GL_DEPTH_TEST);
	}
}

}
}
