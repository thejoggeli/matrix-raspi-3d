#pragma once

#include <string>

namespace Ledlib {

class Vector2f {
public:
	static const Vector2f ZERO;
	static const Vector2f UP;
	static const Vector2f DOWN;
	static const Vector2f LEFT;
	static const Vector2f RIGHT;
    float x, y;
	Vector2f();
	Vector2f(float x, float y);
    void Set(float x, float y);
	void Set(const Vector2f& v);
    void Normalize();
	Vector2f Normalized();
    float GetLength();
    float GetAngle();
    void SetAngle(float a);
    void SetAngle(float a, float length);

    // static functions
	static float GetDistance(Vector2f &v1, Vector2f &v2);
	static float GetAngleBetween(Vector2f &v1, Vector2f &v2);

	void operator = (const Vector2f& v);
	bool operator == (const Vector2f& v) const;
	Vector2f operator + (const Vector2f& v) const;
	Vector2f operator - (const Vector2f& v) const;
	Vector2f operator * (float c) const;
	Vector2f operator / (float c) const;
	void operator += (const Vector2f& v);
	void operator -= (const Vector2f& v);
	void operator *= (float c);
	void operator /= (float c);

	std::string ToString();
};

}
