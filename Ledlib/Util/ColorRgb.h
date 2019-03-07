#pragma once

namespace Ledlib {
class ColorRgb
{
public:
	static const ColorRgb BLACK;
	static const ColorRgb WHITE;
	static const ColorRgb GRAY;
	static const ColorRgb LIGHT_GRAY;
	static const ColorRgb DARK_GRAY;
	static const ColorRgb RED;
	static const ColorRgb GREEN;
	static const ColorRgb BLUE;
	static const ColorRgb YELLOW;
	static const ColorRgb MAGENTA;
	static const ColorRgb CYAN;
    float r = 0;
    float g = 0;
    float b = 0;
    float a = 0;
	ColorRgb();
	ColorRgb(float r, float g, float b);
	~ColorRgb();
    void SetRgb(float r, float g, float b);
    void SetRgba(float r, float g, float b, float);
	void operator = (const ColorRgb& color);
};

}
