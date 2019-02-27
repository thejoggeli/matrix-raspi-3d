#include "Display.h"
#include "../Config.h"
#include "../Log.h"

namespace Ledlib {

Display::Display(){
	brightness = 100;
}
Display::~Display(){}
void Display::SetDimensions(int w, int h){
	width = w;
	height = h;
	numPixels = w*h;
}
int Display::GetBrightness(){
	return brightness;
}
void Display::SetBrightness(int percent){
	brightness = percent;
}
void Display::Shutdown(){}

}

