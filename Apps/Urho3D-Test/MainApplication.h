#pragma once

#include "Ledlib3d/Ledlib3dApplication.h"
#include <memory>

using namespace std;
using namespace Ledlib;
using namespace Urho3D;

namespace Urho3D {
	class Scene;
	class Node;
}

class MainApplication : public Ledlib3dApplication {
public:
	SharedPtr<Scene> scene;
	SharedPtr<Node> boxNode;
	SharedPtr<Node> cameraNode;
	MainApplication(Urho3D::Context * context);
	virtual void OnSetup();
	virtual void OnStart();
	virtual void OnUpdate();
};

URHO3D_DEFINE_APPLICATION_MAIN(MainApplication)
