#pragma once

#include <memory>
#include "Urho3D/Container/Ptr.h"

namespace Urho3D {
class Scene;
class Texture2D;
class Camera;
}

namespace Ledlib {

class App;

class AppManager {
private:
	AppManager();
public:
	static void Setup(App* app);
	static Urho3D::SharedPtr<App> GetApp();
	static Urho3D::SharedPtr<Urho3D::Scene> GetScene();
	static Urho3D::SharedPtr<Urho3D::Texture2D> GetRenderTexture();
	static Urho3D::SharedPtr<Urho3D::Camera> GetCamera();
	static uint8_t* GetPixelBuffer();
};

}


