#include "MatrixDisplay.h"
#include "../Log.h"
#include "../Config.h"
#include <rgbmatrix/led-matrix.h>
#include <string>

using namespace rgb_matrix;

namespace Ledlib {

static RGBMatrix::Options matrixOptions;
static rgb_matrix::RuntimeOptions runtimeOptions;
static std::string pixelMapperString;

MatrixDisplay::MatrixDisplay() {}

bool MatrixDisplay::Init(){
	matrixOptions.cols = Config::GetInt("mat_cols");
	matrixOptions.rows = Config::GetInt("mat_rows");
	matrixOptions.chain_length = Config::GetInt("mat_chain");
	matrixOptions.parallel = Config::GetInt("mat_parallel");
	matrixOptions.brightness = Config::GetInt("mat_brightness");
	matrixOptions.multiplexing = Config::GetInt("mat_multiplexing");
	framerateFraction = static_cast<unsigned int>(Config::GetInt("mat_framerate_fraction"));
	int orientation = Config::GetInt("mat_orientation");
	int rotation = 0;
	if(orientation == 0){
		rotation = Config::GetInt("mat_rotation_landscape");
		SetDimensions(matrixOptions.cols*matrixOptions.chain_length, matrixOptions.rows*matrixOptions.parallel);
	} else {
		rotation = Config::GetInt("mat_rotation_portrait");
		SetDimensions(matrixOptions.rows*matrixOptions.parallel, matrixOptions.cols*matrixOptions.chain_length);
	}
	if(rotation != 0){
		pixelMapperString = "Rotate:" + std::to_string(rotation);
		Log(LOG_DEBUG, "MatrixDisplay", "pixel_mapper_config = \"" + pixelMapperString + "\"");
		matrixOptions.pixel_mapper_config = pixelMapperString.c_str();
	}
	matrix = rgb_matrix::CreateMatrixFromOptions(matrixOptions, runtimeOptions);
	if(matrix == nullptr){
		Log(LOG_ERROR, "MatrixDisplay", "Failed to initialize matrix");
		return false;
	}
	SetBrightness(matrixOptions.brightness);
	offscreen = matrix->CreateFrameCanvas();

	Log(LOG_INFO, "MatrixDisplay", iLog << "Dimensions: " << width << "x" << height);
	Log(LOG_INFO, "MatrixDisplay", iLog << "Orientation: " << (orientation == 0 ? "Landscape" : "Portrait") << ", " << rotation << "deg");
	Log(LOG_INFO, "MatrixDisplay", iLog << "Format: " << "chain_length=" << matrixOptions.chain_length << ", parallel=" << matrixOptions.parallel);
	Log(LOG_INFO, "MatrixDisplay", iLog << "Options: brightness=" << brightness << ", multiplexing=" << matrixOptions.multiplexing << ", framerate_fraction=" << framerateFraction);

	return true;
}

int MatrixDisplay::GetBrightness(){
	return brightness;
}

void MatrixDisplay::SetBrightness(int percent){
	brightness = percent;
	matrix->SetBrightness(static_cast<uint8_t>(percent));
}

void MatrixDisplay::Sync(){
	// draw pixels
	int i = 0;
	for(int y = 0; y < height; y++){
		for(int x = 0; x < width; x++){
			matrix->SetPixel(
				x, y,
				(*pixels)[i],
				(*pixels)[i+1],
				(*pixels)[i+2]
			);
			i += pitch;
		}
	}
	offscreen = matrix->SwapOnVSync(offscreen, framerateFraction);
}

void MatrixDisplay::Shutdown(){
	matrix->Clear();
	delete matrix;
}

}
