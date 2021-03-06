#include "Transform.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <stack>

using namespace glm;

namespace Ledlib {

static mat4 _projectionMatrix = mat4(1.0f);
static mat4 _viewMatrix = mat4(1.0f);
static std::stack<mat4> _transformStack;

namespace Gfx {

const mat4& viewMatrix = _viewMatrix;
const mat4& projectionMatrix = _projectionMatrix;

void Save(){
	_transformStack.push(_viewMatrix);
}
void Restore(){
	_viewMatrix = _transformStack.top();
	_transformStack.pop();
}
void SetTransform(const glm::mat4& m){
	_viewMatrix = m;
}
void ClearTransform(){
	_viewMatrix = glm::mat4(1.0f);
	Scale(1, -1, 1);
	Translate(0.0666f, 0.0666f);
}
void Transform(const mat4& m){
	_viewMatrix = _viewMatrix * m;
}
void Translate(const vec3& v){
	_viewMatrix = translate(_viewMatrix, v);
}
void Translate(float x, float y, float z){
	_viewMatrix = translate(_viewMatrix, vec3(x,y,z));
}
void Scale(float s){
	_viewMatrix = scale(_viewMatrix, vec3(s, s, s));
}
void Scale(float x, float y, float z){
	_viewMatrix = scale(_viewMatrix, vec3(x, y, z));
}
void Rotate(float angle){
	_viewMatrix = rotate(_viewMatrix, angle, vec3(0, 0, 1));
}
void Rotate(float angle, const vec3& axis){
	_viewMatrix = rotate(_viewMatrix, angle, axis);
}
void Rotate(const mat4& m){
	_viewMatrix = _viewMatrix * m;
}
void Rotate(const glm::quat& m){
	_viewMatrix = _viewMatrix * glm::toMat4(m);
}
void SetProjectionMatrix(const mat4& m){
	_projectionMatrix = m;
}

}
}
