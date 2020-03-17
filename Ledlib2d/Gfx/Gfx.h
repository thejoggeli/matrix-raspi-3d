#pragma once

#include <stdint.h>
#include <vector>

namespace Ledlib {

class ColorRgb;
class Bitmap;

namespace Gfx {
	extern float width, height, aspect;
	extern float left, right, bottom, top;
	bool Init();

	// clear
	void Clear();
	void Clear(float r, float g, float b, float a = 1.0);
	void SetClearColor(float r, float g, float b, float a = 1.0f);
	void SetClearColor(const ColorRgb& color);

	void SetRenderTarget(const Bitmap* bitmap);
	void UpdatePixelBuffer();

}

}
