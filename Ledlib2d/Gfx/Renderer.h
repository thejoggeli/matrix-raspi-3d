#pragma once

namespace Ledlib {

class Scene;
class Camera;
class ColorRgb;

namespace Gfx {

void InitRenderer(int width, int height);

// clear
void Clear();
void SetClearColor(float r, float g, float b, float a = 1.0f);
void SetClearColor(const ColorRgb& color);

// render
void SetDepthTestEnabled(bool e);
void Render(Scene* scene, Camera* camera);
void UpdatePixelBuffer();

}
}

