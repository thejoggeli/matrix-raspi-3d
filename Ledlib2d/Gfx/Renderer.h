#pragma once

namespace Ledlib {

class Scene;
class Camera;
class ColorRgb;
class Bitmap;

namespace Gfx {

void SetAutoClear(bool autoClear);

void InitRenderer(int width, int height);

// clear
void Clear();
void Clear(float r, float g, float b, float a = 1.0);
void SetClearColor(float r, float g, float b, float a = 1.0f);
void SetClearColor(const ColorRgb& color);

// render
void SetDepthTestEnabled(bool e);
void RenderPrepare();
void Render(Scene* scene, Camera* camera);
void UpdatePixelBuffer();

void SetRenderTarget(const Bitmap* bitmap);

}
}

