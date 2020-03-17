#pragma once

#include "Shader.h"

namespace Ledlib {
namespace Gfx {
class SolidColorShader : public Shader {
public:
	int position;
	int mvp;
	int color;
};

class ColoredTextureShader : public Shader {
public:
	int position;
	int mvp;
	int color;
	int texture;
	int texCoords;
};

extern SolidColorShader solidColorShader;
extern ColoredTextureShader coloredTextureShader;

void InitShaders();

}
}
