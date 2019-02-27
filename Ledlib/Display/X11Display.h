#pragma once

#include "Display.h"

namespace Ledlib {

class X11Display : public Display {
private:
    int windowWidth;
    int windowHeight;
    int pixelScale = 1;
    int pixelOffset = 1;
    int pixelOffset2 = 2;
public:
	X11Display();
    bool Init();
    void SetPixelScale(int scale);
    void SetPixelOffset(int offset);
	void Sync();
    void Shutdown();
};

}
