#pragma once

#include "Urho3D/Engine/Application.h"
#include "Urho3D/Resource/Image.h"
#include <vector>

namespace Ledlib {

class App : public Urho3D::Application {
public:

	App(Urho3D::Context* context);

	virtual void Setup();
	virtual void Start();
	virtual void Stop();

	void HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
	void HandlePostRenderUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

	virtual void OnSetup(){}
	virtual void OnStart(){}
	virtual void OnUpdate(){}
	virtual void OnStop(){}

};

}


