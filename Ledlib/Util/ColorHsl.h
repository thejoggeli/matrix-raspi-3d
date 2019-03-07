#pragma once

namespace Ledlib {

class ColorRgb;

class ColorHsl {
public:
    static const ColorHsl RED;
    static const ColorHsl GREEN;
    static const ColorHsl BLUE;
    static const ColorHsl CYAN;
    static const ColorHsl MAGENTA;
    static const ColorHsl YELLOW;
    float h = 0.0f;
    float s = 1.0f;
    float l = 1.0f;
    ColorHsl();
    ColorHsl(float h, float s, float l);
    void SetHsl(float h, float s, float l);
    void SetHsl(const ColorHsl* color);
    void AddHue(float amount);
	ColorRgb GetRgb() const;
	static ColorRgb HslToRgb(float h, float s, float l);
	static ColorRgb HslToRgb(const ColorHsl* color);
    static float HueToRgb(float p, float q, float t);
    static float WrapHue(float hue);
};

}
