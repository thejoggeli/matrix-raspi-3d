#pragma once

#include "Ledlib/Glue/Urho3D/Urho3DApplication.h"
#include <memory>

using namespace std;
using namespace Ledlib;
using namespace Urho3D;

namespace Urho3D {
	class Scene;
	class Node;
}

class MainApplication : public Urho3DApplication {
public:
	SharedPtr<Scene> scene;
	SharedPtr<Node> box;
	SharedPtr<Node> camera;
	MainApplication(Urho3D::Context * context);
	virtual void Start();
	virtual void HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap& eventData);
};

URHO3D_DEFINE_APPLICATION_MAIN(MainApplication)
