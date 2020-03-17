#pragma once

#include <string>

namespace Ledlib {
namespace Gfx {

// rectangle shape
struct RectangleShape {
	unsigned int vertices_vbo = 0;
	float vertices[12] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
	0.5f,  0.5f, 0.0f
	};
	unsigned int uvs_vbo = 0;
	float uvs[8] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	};
};

// polygon shape
struct PolygonShape {
	unsigned int vbo = 0;
	float vertices[3*1024];
};

// point shaoe
struct PointShape {
	unsigned int vbo = 0;
	float vertices[3] = {0.0f, 0.0f, 0.0f};
};

extern RectangleShape rectangleShape;
extern PolygonShape polygonShape;
extern PointShape pointShape;

bool InitOpenGL(int width, int height);
bool PrintOpenGLError(const std::string& context);
const std::string GetEglErrorString(int error);

bool GenerateTextureFromFramebuffer(unsigned int *t, unsigned int *f, unsigned int w, unsigned int h);

}
}
