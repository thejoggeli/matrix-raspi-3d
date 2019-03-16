#pragma once

#include "Ledlib2d/Entity.h"

using namespace Ledlib;

namespace Ledlib {
class Bitmap;
class ShaderBox;
class BaseFont;
}

class Clock : public Entity {
public:
	float nextEffectTime;
	int offsetHours = 0;
	float impulse = 0.0f;
	float spin = 0.0f;
	glm::vec3 extraScale = glm::vec3(1.0f);
	std::shared_ptr<Bitmap> bitmap;
	std::shared_ptr<ShaderBox> shader;
	int fontId = 0;
	BaseFont* font;
	Clock();
	void OnStart();
	void OnUpdate();
	void OnRender();
	void OnEnd();
	void RenderToTexture();
	void ResetEffectTime();
};

