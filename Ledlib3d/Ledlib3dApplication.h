#pragma once

#include "Urho3D/Container/Ptr.h"
#include "Urho3D/Engine/Application.h"
#include "Urho3D/Resource/Image.h"
#include "Urho3D/Scene/Scene.h"
#include "Urho3D/Graphics/Graphics.h"
#include "Urho3D/Graphics/Octree.h"
#include "Urho3D/Graphics/Texture2D.h"
#include "Urho3D/Graphics/Viewport.h"
#include "Urho3D/Graphics/RenderSurface.h"
#include "Urho3D/Graphics/Camera.h"
#include <vector>

namespace Urho3D {
	class Scene;
	class Camera;
	class Texture2D;
}

namespace Ledlib {

class Ledlib3dApplication : public Urho3D::Application {
private:

	Urho3D::SharedPtr<Urho3D::Scene> _scene;
	Urho3D::SharedPtr<Urho3D::Camera> _camera;
	Urho3D::SharedPtr<Urho3D::Texture2D> _renderTexture;
	uint8_t* _pixels;

public:


	Ledlib3dApplication(Urho3D::Context* context);

	virtual void Setup();
	virtual void Start();
	virtual void Stop();

	void HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
	void HandlePostRenderUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

	virtual void OnSetup(){}
	virtual void OnStart(){}
	virtual void OnUpdate(){}
	virtual void OnStop(){}

	Urho3D::SharedPtr<Urho3D::Scene> GetScene();
	Urho3D::SharedPtr<Urho3D::Texture2D> GetRenderTexture();
	Urho3D::SharedPtr<Urho3D::Camera> GetCamera();
	uint8_t* GetPixelBuffer();

};

}

