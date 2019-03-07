#pragma once

namespace Ledlib {
namespace Gfx {

class Shader {
public:
	int id = 0;
	int fragId = 0;
	int vertId = 0;
	Shader();
	void Use();
	virtual ~Shader();
};

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
