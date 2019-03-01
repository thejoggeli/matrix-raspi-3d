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
	int initCounter = 0;
	int width, height;
	int numPixels;
	int pitch;
	uint8_t* pixels;
	Display();
	virtual ~Display();
	virtual bool Init();
	virtual void Sync();
	virtual int GetBrightness();
	virtual void SetBrightness(int percent);
	virtual void Shutdown();
};

}

