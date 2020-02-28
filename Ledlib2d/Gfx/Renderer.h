#pragma once

namespace Ledlib {

class Scene;
class Camera;
class ColorRgb;
class Bitmap;

namespace Gfx {

void InitRenderer(int width, int height);
void SetAutoClear(bool autoClear);
void SetDepthTestEnabled(bool e);
void RenderPrepare();
void Render(Scene* scene, Camera* camera);

}
}

