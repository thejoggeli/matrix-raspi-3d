#pragma once

#include "Ledlib3d/App.h"
#include <memory>

using namespace std;
using namespace Ledlib;
using namespace Urho3D;

namespace Urho3D {
	class Scene;
	class Node;
}

class MainApplication : public App {
public:
	SharedPtr<Scene> scene;
	SharedPtr<Node> boxNode;
	SharedPtr<Node> cameraNode;
	MainApplication(Urho3D::Context * context);
	virtual void OnStart();
	virtual void OnUpdate();
};

URHO3D_DEFINE_APPLICATION_MAIN(MainApplication)
