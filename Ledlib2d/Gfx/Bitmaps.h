#pragma once

class ColorRgb;

namespace Ledlib {
namespace Gfx {

class Bitmap;

void SetBitmapColor(float r, float g, float b, float a = 1.0f);
void SetBitmapColor(const ColorRgb& color);

// bitmap
void DrawBitmap(const Bitmap* bitmap, float x, float y, float rotation = 0);
void DrawBitmap(const Bitmap* bitmap, float x, float y, float w, float h, float rotation = 0);
// void DrawBitmap(const Bitmap* bitmap, float x, float y, float w, float h, const Rect4f* rsrc, float rotation = 0);

}
}

