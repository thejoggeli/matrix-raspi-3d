#pragma once

#include "Urho3D/Container/Ptr.h"
#include "Urho3D/Engine/Application.h"
#include "Urho3D/Graphics/Texture2D.h"
#include "Urho3D/Graphics/Camera.h"
#include "Urho3D/Scene/Scene.h"
#include <vector>

namespace Ledlib {

class Ledlib3dApplication : public Urho3D::Application {
private:

	Urho3D::SharedPtr<Urho3D::Scene> _scene;
	Urho3D::SharedPtr<Urho3D::Camera> _camera;
	Urho3D::SharedPtr<Urho3D::Texture2D> _renderTexture;
	uint8_t* _pixels;

public:


	Ledlib3dApplication(Urho3D::Context* context);

	// these are called automatically by Urho3D
	virtual void Setup();
	virtual void Start();
	virtual void Stop();
	void HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
	void HandlePostRenderUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

	// these are called "manually"
	virtual void OnSetup(){}
	virtual void OnStart(){}
	virtual void OnUpdate(){}
	virtual void OnExit(){}

	Urho3D::SharedPtr<Urho3D::Scene> GetScene();
	Urho3D::SharedPtr<Urho3D::Texture2D> GetRenderTexture();
	Urho3D::SharedPtr<Urho3D::Camera> GetCamera();
	uint8_t* GetPixelBuffer();

};

}


