#include "ColorRgb.h"


namespace Ledlib {

const ColorRgb ColorRgb::BLACK = ColorRgb(0, 0, 0);
const ColorRgb ColorRgb::WHITE = ColorRgb(1.0f, 1.0f, 1.0f);
const ColorRgb ColorRgb::GRAY = ColorRgb(0.5f, 0.5f, 0.5f);
const ColorRgb ColorRgb::LIGHT_GRAY = ColorRgb(0.75f, 0.75f, 0.75f);
const ColorRgb ColorRgb::DARK_GRAY = ColorRgb(0.25f, 0.25f, 0.25f);
const ColorRgb ColorRgb::RED = ColorRgb(1.0f, 0, 0);
const ColorRgb ColorRgb::GREEN = ColorRgb(0, 1.0f, 0);
const ColorRgb ColorRgb::BLUE = ColorRgb(0, 0, 1.0f);
const ColorRgb ColorRgb::YELLOW = ColorRgb(1.0f, 1.0f, 0);
const ColorRgb ColorRgb::MAGENTA = ColorRgb(1.0f, 0, 1.0f);
const ColorRgb ColorRgb::CYAN = ColorRgb(0, 1.0f, 1.0f);

ColorRgb::ColorRgb(){}
ColorRgb::ColorRgb(float r, float g, float b) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 1.0f;
}

ColorRgb::~ColorRgb(){
}

void ColorRgb::SetRgb(float r, float g, float b){
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 1.0f;
}

void ColorRgb::SetRgba(float r, float g, float b, float a){
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

void ColorRgb::operator = (const ColorRgb& ColorRgb){
	r = ColorRgb.r;
	g = ColorRgb.g;
	b = ColorRgb.b;
	a = ColorRgb.a;
}
}

