#include "Vector2f.h"
#include <math.h>
#include <string.h>

namespace Ledlib {

const Vector2f Vector2f::ZERO = Vector2f(0, 0);
const Vector2f Vector2f::UP = Vector2f(0, -1);
const Vector2f Vector2f::DOWN = Vector2f(0, 1);
const Vector2f Vector2f::LEFT = Vector2f(-1, 0);
const Vector2f Vector2f::RIGHT = Vector2f(1, 0);

Vector2f::Vector2f() : Vector2f(0, 0){}

Vector2f::Vector2f(float x, float y){
	this->x = x;
	this->y = y;
}

void Vector2f::Set(float x, float y){
	this->x = x;
	this->y = y;
}
void Vector2f::Set(const Vector2f& v){
	x = v.x;
	y = v.y;
}

float Vector2f::GetLength(){
	return sqrtf(x*x+y*y);
}

float Vector2f::GetAngle(){
	return atan2f(y,x);
}

void Vector2f::SetAngle(float a){
	float length = GetLength();
	x = cosf(a) * length;
	y = sinf(a) * length;
}

void Vector2f::SetAngle(float a, float length){
	x = cosf(a) * length;
	y = sinf(a) * length;
}

void Vector2f::Normalize(){
	if(*this == Vector2f::ZERO) return;
	float len = GetLength();
	x /= len;
	y /= len;
}
Vector2f Vector2f::Normalized(){
	if(*this == Vector2f::ZERO) return Vector2f::ZERO;
	float len = GetLength();
	return Vector2f(x/len, y/len);
}

float Vector2f::GetAngleBetween(Vector2f &v1, Vector2f &v2){
	return atan2f(v1.y-v2.y, v1.x-v2.x);
}

float Vector2f::GetDistance(Vector2f &v1, Vector2f &v2){
	float x = v1.x - v2.x;
	float y = v1.y - v2.y;
	return sqrtf(x*x+y*y);
}

void Vector2f::operator = (const Vector2f& v) {
	x = v.x;
	y = v.y;
}
bool Vector2f::operator == (const Vector2f& v) const {
	return this->x == v.x && this->y == v.y;
}
Vector2f Vector2f::operator - (const Vector2f& v) const {
	return Vector2f(x - v.x, y - v.y);
}
void Vector2f::operator -= (const Vector2f& v){
	x -= v.x;
	y -= v.y;
}
Vector2f Vector2f::operator + (const Vector2f& v) const {
	return Vector2f(x + v.x, y + v.y);
}
void Vector2f::operator += (const Vector2f& v){
	x += v.x;
	y += v.y;
}
Vector2f Vector2f::operator * (float c) const {
	return Vector2f(x*c, y*c);
}
void Vector2f::operator *= (float c){
	x *= c;
	y *= c;
}
Vector2f Vector2f::operator / (float c) const {
	return Vector2f(x/c, y/c);
}
void Vector2f::operator /= (float c){
	x /= c;
	y /= c;
}

std::string Vector2f::ToString(){
	return "Vector2f{x:" + std::to_string(x) + " / " + std::to_string(y) + "}";
}

}
