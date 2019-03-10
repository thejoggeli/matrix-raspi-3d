#include "Draw.h"
#include "Ledlib/Util/ColorRgb.h"
#include "OpenGL.h"
#include "Shaders.h"
#include <GLES2/gl2.h>
#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Path.h"

namespace Ledlib {

static ColorRgb _drawColor;

// static glm::mat4 _screenCoordinatesMatrix = glm::mat4(1.0f);

namespace Gfx {

const ColorRgb& drawColor = _drawColor;

float lineWidth = 1.0f;
StrokeAlign strokeAlign = StrokeAlign::Inside;

void InitDraw(int screenWidth, int screenHeight){
/*	_screenCoordinatesMatrix = glm::ortho(-screenWidth/2.0f, screenWidth/2.0f, -screenHeight/2.0f, screenHeight/2.0f, -1.0f, 1.0f);
	_screenCoordinatesMatrix = glm::scale(_screenCoordinatesMatrix, glm::vec3(1, -1, 1));
	_screenCoordinatesMatrix = glm::translate(_screenCoordinatesMatrix, glm::vec3(0.5f, 0.5f, 0));
	_screenCoordinatesMatrix = glm::translate(_screenCoordinatesMatrix, glm::vec3(-screenWidth/2.0f, -screenHeight/2.0f, 0)); */
}

void SetDrawColor(float r, float g, float b, float a){
	_drawColor.r = r; _drawColor.g = g; _drawColor.b = b; _drawColor.a = a;
}
void SetDrawColor(const ColorRgb& color){
	_drawColor = color;
}

// pixel
void SetPixel(int x, int y){
	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, 1, 1);
	glClearColor(_drawColor.r, _drawColor.g, _drawColor.b, _drawColor.a);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

}
void SetPixel(int x, int y, const ColorRgb& color){
	glEnable(GL_SCISSOR_TEST);
	glScissor(x, y, 1, 1);
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
}

// rectangle
void StrokeRect(float x, float y, float w, float h){
	float w2 = w/2.0f;
	float h2 = h/2.0f;
	float off = lineWidth/2.0f;
	float off2 = lineWidth;
	if(strokeAlign == StrokeAlign::Inside){
		StrokeLine(x-w2+off, y-h2, x-w2+off, y+h2);
		StrokeLine(x-w2+off2, y+h2-off, x+w2-off2, y+h2-off);
		StrokeLine(x+w2-off, y+h2, x+w2-off, y-h2);
		StrokeLine(x+w2-off2, y-h2+off, x-w2+off2, y-h2+off);
	} else {
		StrokeLine(x-w2-off, y-h2-off2, x-w2-off, y+h2+off2);
		StrokeLine(x-w2, y+h2+off, x+w2, y+h2+off);
		StrokeLine(x+w2+off, y+h2+off2, x+w2+off, y-h2-off2);
		StrokeLine(x+w2, y-h2-off, x-w2, y-h2-off);
	}
}
void FillRect(float x, float y, float w, float h, float rotation){
	solidColorShader.Use();
	glUniform4f(solidColorShader.color, _drawColor.r, _drawColor.g, _drawColor.b, _drawColor.a);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0));
	if(rotation != 0.0f){
		model = glm::rotate(model, rotation, glm::vec3(0, 0, 1));
	}
	model = glm::scale(model, glm::vec3(w, h, 1.0f));
	glm::mat4 mvp = projectionMatrix * viewMatrix * model;
	glUniformMatrix4fv(solidColorShader.mvp, 1, GL_FALSE, glm::value_ptr(mvp));
	glEnableVertexAttribArray(solidColorShader.position);
	glBindBuffer(GL_ARRAY_BUFFER, rectangleShape.vertices_vbo);
	glVertexAttribPointer(solidColorShader.position, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

// line
void StrokeLine(float x1, float y1, float x2, float y2){
	float xx = x1-x2;
	float yy = y1-y2;
	float length = sqrtf(xx*xx+yy*yy);
	float angle = atan2f(y2-y1, x2-x1);
	FillRect((x1+x2)/2, (y1+y2)/2, length, lineWidth, angle);
}

void FillCircle(float x, float y, float radius, int numVertices){
	if(numVertices == 0){
		numVertices = static_cast<int>(radius*4.0f);
		if(numVertices < 8){
			numVertices = 8;
		}
	}
	BeginPath();
	float angle = 0;
	float angleStep = 3.14159265359f*2.0f/numVertices;
	for(int i = 0; i < numVertices; i++){
		angle += angleStep;
		LineTo(sinf(angle)*radius+x, cosf(angle)*radius+y);
	}
	ClosePath();
	FillPath();
}

}
}
