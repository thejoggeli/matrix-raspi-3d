#pragma once


namespace Ledlib {
namespace Numbers {

const float Pi = 3.14159265358979323846f;
const float RadToDeg = 57.2957795131f;
const float DegToRad = 0.01745329251f;

bool Init();
float Mod(float a, float b);
int Wrap(int min, int max, int val);
float Wrap(float min, float max, float val);
int Clamp(int min, int max, int val);
float Clamp(float min, float max, float val);
int Random(int min, int max);
float Random(float min, float max);
int Max(int a, int b, int c);
int Min(int a, int b, int c);
float Max(float a, float b);
float Min(float a, float b);
float Max(float a, float b, float c);
float Min(float a, float b, float c);
float Stepify(float value, float step);
float RotationStep(float rotation, int steps);

}
};
