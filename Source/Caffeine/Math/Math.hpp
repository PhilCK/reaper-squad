
#ifndef CAFFEINE_MATH_MATH_INCLUDED
#define CAFFEINE_MATH_MATH_INCLUDED

#include <Caffeine/Platform.hpp>
#include <math.h>
#include <algorithm>

namespace Caffeine {
namespace Math {

// Math constants
const float PI		= 3.14159265359f;
const float TAU		= 2.0f * PI;
const float E		= 2.71828f;
const float GRATIO  = 1.61803398875f;
const float ROOT2	= 1.41421356237f;
const float ROOT3   = 1.73205080757f;


#pragma mark - Trig Functions

inline float Sin(const float x) {
	return sin(x);
}

inline float ASin(const float x) {
	return asin(x);
}

inline float Cos(const float x) {
	return cos(x);
}

inline float ACos(const float x) {
	return acos(x);
}

inline float Tan(const float x) {
	return tan(x);
}

inline float ATan(const float x) {
	return atan(x);
}

inline float ATan2(const float x, const float y) {
	return atan2f(x, y);
}


#pragma mark - Log and Exp

float Log2(const float x);

float Log10(const float x);

float Exp(const float x);

float Pow(const float x, const float pow);


#pragma mark - Min, Max, Clamp, Round

inline float Max(const float x, const float y) {
	return fmaxf(x, y);
}

inline int Max(const int x, const int y) {
	return std::max(x, y);
}

inline float Min(const float x, const float y) {
	return fminf(x, y);
}

inline int Min(const int x, const int y) {
	return std::min(x, y);
}

inline float Clamp(const float a, const float x, const float y) {
	return a < x ? x : (a > y ? y : a);
}

inline int Clamp(const int a, const int x, const int y) {
	return a < x ? x : (a > y ? y : a);
}

float Ceil(const float x);

float Floor(const float x);


#pragma mark - Degrees and Radians

inline float DegToRad(const float x) {
	return x * (PI / 180);
}

inline float RadToDeg(const float x) {
	return x * (180 / PI);
}


#pragma mark - Other

inline float SquareRoot(const float x) {
	return (float)sqrt(x);
}

float Abs(const float x);

int Abs(const int x);

    
} // namespace
} // namespace

#endif // include guard