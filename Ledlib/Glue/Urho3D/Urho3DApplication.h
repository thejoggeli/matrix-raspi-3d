#pragma once

#include "Urho3D/Engine/Application.h"
#include "Urho3D/Resource/Image.h"
#include <vector>


namespace Ledlib {

class Urho3DApplication : public Urho3D::Application {
public:
	int width, height, pitch;
	uint32_t* pixels;
	Urho3D::SharedPtr<Urho3D::Image> pixelsImage;
	Urho3DApplication(Urho3D::Context * context);

	virtual void Setup();
	virtual void Start();
	virtual void Stop();
	virtual void HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
	virtual void HandlePostRenderUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
	virtual void HandleEndFrame(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);

};

}


