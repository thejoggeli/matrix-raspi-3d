#pragma once

#include "Display.h"

namespace rgb_matrix {
    class RGBMatrix;
    class FrameCanvas;
}

namespace Ledlib {

class MatrixDisplay : public Display {
	private:
		int brightness;
		rgb_matrix::RGBMatrix* matrix;
		rgb_matrix::FrameCanvas* offscreen;
		unsigned int framerateFraction = 0;
	public:
		MatrixDisplay();
		bool Init();
		void Sync();
		int GetBrightness();
		void SetBrightness(int percent);
		void Shutdown();
	};

}
