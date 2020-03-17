#include "ColorHsl.h"
#include "ColorRgb.h"
#include <math.h>

namespace Ledlib {

const ColorHsl ColorHsl::RED		= ColorHsl(0.0f/6.0f, 1, 0.5f);
const ColorHsl ColorHsl::GREEN		= ColorHsl(2.0f/6.0f, 1, 0.5f);
const ColorHsl ColorHsl::BLUE		= ColorHsl(4.0f/6.0f, 1, 0.5f);
const ColorHsl ColorHsl::CYAN		= ColorHsl(3.0f/6.0f, 1, 0.5f);
const ColorHsl ColorHsl::MAGENTA	= ColorHsl(1.0f/6.0f, 1, 0.5f);
const ColorHsl ColorHsl::YELLOW		= ColorHsl(5.0f/6.0f, 1, 0.5f);

ColorHsl::ColorHsl(){}

ColorHsl::ColorHsl(float h, float s, float l){
	this->h = h;
	this->s = s;
	this->l = l;
}

void ColorHsl::SetHsl(float h, float s, float l){
	this->h = h;
	this->s = s;
	this->l = l;
}
void ColorHsl::SetHsl(const ColorHsl* color){
	SetHsl(color->h, color->s, color->l);
}

void ColorHsl::AddHue(float amount){
	h = ColorHsl::WrapHue(h + amount);
}

float ColorHsl::WrapHue(float hue){
	return fmodf(hue, 1.0f);
}

ColorRgb ColorHsl::GetRgb() const {
	return HslToRgb(h, s, l);
}

ColorRgb ColorHsl::HslToRgb(float h, float s, float l){
	float r, g, b;
	if(s == 0.0f){
		r = g = b = l; // achromatic
	} else {
		float q = l < 0.5f ? l * (1.0f + s) : l + s - l * s;
		float p = 2.0f * l - q;
		r = ColorHsl::HueToRgb(p, q, h + 1.0f/3.0f);
		g = ColorHsl::HueToRgb(p, q, h);
		b = ColorHsl::HueToRgb(p, q, h - 1.0f/3.0f);
	}
	return ColorRgb(r, g, b);
}
ColorRgb ColorHsl::HslToRgb(const ColorHsl* color){
	return HslToRgb(color->h, color->s, color->l);
}

float ColorHsl::HueToRgb(float p, float q, float t){
	if(t < 0.0f) t += 1.0f;
	if(t > 1.0f) t -= 1.0f;
	if(t < 1.0f/6.0f) return p + (q - p) * 6.0f * t;
	if(t < 1.0f/2.0f) return q;
	if(t < 2.0f/3.0f) return p + (q - p) * (2.0f/3.0f - t) * 6.0f;
	return p;
}

}
