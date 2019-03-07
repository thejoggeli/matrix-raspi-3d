#include "Path.h"
#include "OpenGL.h"
#include "Shaders.h"
#include <GLES2/gl2.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Transform.h"
#include "Draw.h"
#include "Ledlib/Util/ColorRgb.h"

namespace Ledlib {
namespace Gfx {

// polygon
static int pathIndex = 0;
void BeginPath(){
	pathIndex = 0;
}
void LineTo(float x, float y, float z){
	polygonShape.vertices[pathIndex] = x;
	polygonShape.vertices[pathIndex+1] = y;
	polygonShape.vertices[pathIndex+2] = z;
	pathIndex += 3;
}
void ClosePath(){
	glBindBuffer(GL_ARRAY_BUFFER, polygonShape.vbo);
	glBufferData(GL_ARRAY_BUFFER, pathIndex*sizeof(float), polygonShape.vertices, GL_DYNAMIC_DRAW);
}

void StrokePath(){
	// todo
}

void FillPath(){
	solidColorShader.Use();
	glm::mat4 mvp = projectionMatrix * viewMatrix;
	glUniform4f(solidColorShader.color, drawColor.r, drawColor.g, drawColor.b, drawColor.a);
	glUniformMatrix4fv(solidColorShader.mvp, 1, GL_FALSE, glm::value_ptr(mvp));
	glEnableVertexAttribArray(solidColorShader.position);
	glVertexAttribPointer(solidColorShader.position, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, polygonShape.vbo);
	glDrawArrays(GL_TRIANGLE_FAN, 0, pathIndex/3);
}



}

}
