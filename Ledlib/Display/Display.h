#pragma once

#include <stdint.h>
#include <vector>

namespace Ledlib {

class Display {
private:
	int brightness;
protected:
	void SetDimensions(int width, int height);
public:
	int width, height;
	int numPixels;
	int pitch;
	std::vector<uint8_t>* pixels;
	Display();
	virtual ~Display();
	virtual bool Init() = 0;
	virtual void Sync() = 0;
	virtual int GetBrightness();
	virtual void SetBrightness(int percent);
	virtual void Shutdown();
};

}

