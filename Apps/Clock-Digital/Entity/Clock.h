#pragma once

#include "Ledlib2d/Entity.h"

using namespace Ledlib;

namespace Ledlib {
class Bitmap;
class ShaderBox;
}

class Clock : public Entity {
public:
	std::shared_ptr<Bitmap> bitmap;
	std::shared_ptr<ShaderBox> shader;
	Clock();
	void OnStart();
	void OnUpdate();
	void OnRender();
	void OnEnd();
	void RenderToTexture();
};

