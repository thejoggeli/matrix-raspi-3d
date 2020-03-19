#include "AppCamera.h"
#include "Ledlib/Log.h"
#include "Ledlib/Remote/Client.h"
#include "Ledlib/Remote/ClientManager.h"
#include "Ledlib/Math/Numbers.h"
#include "Ledlib/Time.h"
#include "Gfx/Transform.h"
#include "Gfx/Gfx.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

using namespace glm;

namespace Ledlib {

AppCamera::AppCamera(){
	SetOrthographic();
}

void AppCamera::Reset(){
	SetPosition(0, 0, 0);
	SetRotation(0);
	SetScale(1);
}

void AppCamera::SimpleStartReset(){
	if(ClientManager::IsKeyDown(KeyCode::Start)){
		Reset();
	}
}

void AppCamera::SimpleSelectReset(){
	if(ClientManager::IsKeyDown(KeyCode::Select)){
		Reset();
	}
}

void AppCamera::SimpleJoystickMove(float sensitivity){
	for(auto& client: ClientManager::GetAllCients()){
		if(client->IsKeyDown(KeyCode::LeftJoystick)){
			Vector2f joy = client->GetJoystickPosition(KeyCode::LeftJoystick);
			glm::vec3 dir = rotation * glm::vec3(joy.x, joy.y, 0);
			dir *= sensitivity;
			dir *= this->scale;
			this->Translate(dir * Time::deltaTime);
			break;
		}
	}
}
void AppCamera::SimpleJoystickRotate(float sensitivity){
	for(auto& client: ClientManager::GetAllCients()){
		if(client->IsKeyDown(KeyCode::LeftJoystick)){
			Vector2f joy = client->GetJoystickPosition(KeyCode::LeftJoystick);
			joy *= sensitivity;
			this->Rotate(Numbers::Pi * Time::deltaTime * -joy.x);
			break;
		}
	}
}
void AppCamera::SimpleJoystickZoom(float sensitivity){
	for(auto& client: ClientManager::GetAllCients()){
		if(client->IsKeyDown(KeyCode::LeftJoystick)){
			Vector2f joy = client->GetJoystickPosition(KeyCode::LeftJoystick);
			joy *= sensitivity;
			this->SetScale(this->scale.x + this->scale.x * Time::deltaTime * -joy.y);
			break;
		}
	}
}
void AppCamera::SimpleArrowsMove(float sensitivity){
	glm::vec3 dir(0, 0, 0);
	if(ClientManager::IsKeyDown(KeyCode::Left)) dir.x = -1;
	if(ClientManager::IsKeyDown(KeyCode::Right)) dir.x = 1;
	if(ClientManager::IsKeyDown(KeyCode::Up)) dir.y = 1;
	if(ClientManager::IsKeyDown(KeyCode::Down)) dir.y = -1;
	if(glm::length(dir) != 0.0f){
		dir = glm::normalize(dir);
		dir = rotation * dir;
		dir *= this->scale;
		this->Translate(dir * Time::deltaTime * sensitivity);
	}
}
void AppCamera::SimpleArrowsRotate(float sensitivity){
	if(ClientManager::IsKeyDown(KeyCode::Left)){
		this->Rotate(Numbers::Pi * Time::deltaTime * sensitivity);
	}
	if(ClientManager::IsKeyDown(KeyCode::Right)){
		this->Rotate(-Numbers::Pi * Time::deltaTime * sensitivity);
	}
}
void AppCamera::SimpleArrowsZoom(float sensitivity){
	if(ClientManager::IsKeyDown(KeyCode::Up)){
		this->SetScale(this->scale.x - this->scale.x * Time::deltaTime * sensitivity);
	}
	if(ClientManager::IsKeyDown(KeyCode::Down)){
		this->SetScale(this->scale.x + this->scale.x * Time::deltaTime * sensitivity);
	}
}
void AppCamera::ApplyTransform(){
	UpdateTransformMatrix();
	Gfx::Transform(glm::inverse(transformMatrix));
}
void AppCamera::UpdateTransformMatrix(){
	transformMatrix = mat4(1.0f);
	transformMatrix = glm::translate(transformMatrix, position);
	transformMatrix = transformMatrix * glm::toMat4(rotation);
	transformMatrix = glm::scale(transformMatrix, scale);
}

void AppCamera::SetPerspective(float fovy){
	SetPerspective(fovy, Gfx::width/Gfx::height, 0.01f, 100000.0f);
}
void AppCamera::SetPerspective(float fovy, float aspect, float near, float far){
	projectionMatrix = glm::perspective(fovy, aspect, near, far);
	projectionMatrix = glm::rotate(projectionMatrix, Numbers::Pi, glm::vec3(0, 0, 1));
}

void AppCamera::SetOrthographic(){
	SetOrthographic(Gfx::left, Gfx::right, Gfx::bottom, Gfx::top, -100000, 100000);
}

void AppCamera::SetOrthographic(float left, float right, float bottom, float top, float near, float far){
	projectionMatrix = glm::ortho(left, right, bottom, top, near, far);
}

void AppCamera::SetPosition(float x, float y, float z){
	position.x = x;
	position.y = y;
	position.z = z;
}
void AppCamera::SetPosition(const vec3& v){
	position = v;
}
void AppCamera::SetPosition(const vec2& v){
	position.x = v.x;
	position.y = v.y;
}
void AppCamera::Translate(float x, float y, float z){
	position.x += x;
	position.y += y;
	position.z += z;
}
void AppCamera::Translate(const glm::vec3 &v){
	position += v;
}
void AppCamera::Translate(const glm::vec2 &v){
	position.x += v.x;
	position.y += v.y;
}
void AppCamera::Move(float x, float y, float z){
	position += rotation * glm::vec3(x, y, z);
}
void AppCamera::Move(const glm::vec3& v){
	position += rotation * v;
}
void AppCamera::Move(const glm::vec2& v){
	position += rotation * glm::vec3(v.x, v.y, 0);
}

void AppCamera::SetScale(float s){
	scale.x = s;
	scale.y = s;
	scale.z = s;
}
void AppCamera::SetScale(float x, float y, float z){
	scale.x = x;
	scale.y = y;
	scale.z = z;
}
void AppCamera::SetScale(const vec3& v){
	scale = v;
}
void AppCamera::Scale(float s){
	scale.x *= s;
	scale.y *= s;
	scale.z *= s;
}
void AppCamera::Scale(float x, float y, float z){
	scale.x *= x;
	scale.y *= y;
	scale.z *= z;
}
void AppCamera::Scale(const glm::vec3& v){
	scale.x *= v.x;
	scale.y *= v.y;
	scale.z *= v.z;
}

void AppCamera::SetRotation(float z){
	rotation = glm::angleAxis(z, vec3(0, 0, 1));
}
void AppCamera::SetRotation(const mat4& r){
	rotation = r;
}
void AppCamera::SetRotation(const quat& r){
	rotation = r;
}
void AppCamera::SetRotation(const glm::vec3 eulers){
	rotation = glm::eulerAngleYXZ(eulers.y, eulers.x, eulers.z);
}
glm::vec3 AppCamera::GetEulers(){
	return glm::eulerAngles(rotation);
}
void AppCamera::Rotate(float z){
	rotation = glm::rotate(rotation, z, vec3(0,0,1));
}
void AppCamera::Rotate(const glm::quat& r){
	rotation = rotation * r;
}
float AppCamera::GetAngle(){
	return glm::eulerAngles(rotation)[2];
}


}

