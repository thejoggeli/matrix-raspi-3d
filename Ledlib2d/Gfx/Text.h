#pragma once

#include <string>

namespace Ledlib {

class BaseFont;
class ColorRgb;

enum class TextBaseline {TOP, MIDDLE, ALPHABETIC, BOTTOM};
enum class TextAlign {LEFT, CENTER, RIGHT};

namespace Gfx {

extern bool antiKerning;
extern TextBaseline textBaseline;
extern TextAlign textAlign;
extern int textCharacterSpacing;

void SetTextColor(float r, float g, float b, float a = 1);
void SetTextColor(const ColorRgb& color);

// text
void SetFont(BaseFont* font);
void SetTextPosition(TextAlign align, TextBaseline baseline);
void DrawText(std::string str, float x, float y);

}

}
