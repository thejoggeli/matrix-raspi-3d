#pragma once

namespace Ledlib {

class ColorRgb;

enum class StrokeAlign {Inside, Outside};

namespace Gfx {

extern const ColorRgb& drawColor;
extern float lineWidth;
extern StrokeAlign strokeAlign;

void InitDraw(int screenWidth, int screenHeight);

void SetDrawColor(float r, float g, float b, float a = 1.0f);
void SetDrawColor(const ColorRgb& color);

// pixel
void SetPixel(int x, int y);
void SetPixel(int x, int y, const ColorRgb& color);

// rectangle
void StrokeRect(float x, float y, float w, float h);
void FillRect(float x, float y, float w, float h, float rotation = 0);

// line
void StrokeLine(float x1, float y1, float x2, float y2);

// circle
void FillCircle(float x, float y, float radius, int numVertices = 0);
void StrokeCircle(float x, float y, float radius, int numVertices = 0);

}

}
