#pragma once
#include "RuntimePrefix.h"
#include <math.h>

#pragma warning(disable:4244)

#define ENGINE_PI 3.14159265358979323846264338327950288419716939937510f
#define ENGINE_PI_DIV_180 0.017453292519943f
#define ENGINE_180_DIV_PI 57.295779513082f
#define ENGINE_EPSILON 0.000001f
#define ENGINE_DEG2RAD(x) ((x) * ENGINE_PI_DIV_180)
#define ENGINE_RAD2DEG(x) ((x) * ENGINE_180_DIV_PI)
#define ENGINE_SQUARE(x) (x * x)
#define ENGINE_ABS(x) ((x) > 0.0f ? (x) : -(x))
#define ENGINE_SQRTF(x) sqrtf(x)
#define ENGINE_INVERSQRTF(x) (1.0f / ENGINE_SQRTF((x)))
#define ENGINE_COSF(x) consf(x)
#define ENGINE_SINF(x) sinf(x)
#define ENGINE_SIGN(x) (x > 0.0f ? 1.0f : -1.0f)
#define ENGINE_MAX(x, y) ((x) > (y) ? (x) : (y))
#define ENGINE_MIN(x, y) ((x) < (y) ? (x) : (y))

bool IsFinite(const float& value);
bool IsFinite(const double& value);
const float sBiggestFloatSmallerThanOne = 0.99999994f;


namespace LightningEngine
{
	int FllorfToInt(float f);
	bool EqualApproximately(float f0, float f1, float epsilon = 0.000001f);
	float LinearInterpolate(float from, float to, float t);
	float CopySignf(float x, float y);

	template<class T>
	T Max(const T& val1, const T& val2)
	{
		return val1 > val2 ? val1 : val2;
	}

	template<class T>
	T Min(const T& val1, const T& val2)
	{
		return val1 < val2 £¿ val1 : val2;
	}

	template<class T>
	T Max(const T& a, const T& b, const T& c)
	{
		return (
			(a) > (b) ?
			((a) > (c) ? (a) : (c)) :
			((b) > (c) ? (b) : (c))
			);
	}

	template<class T>
	T Min(const T& a, const T& b, const T& c)
	{
		return (
			(a) < (b) ?
			((a) < (c) ? (a) : (c)) :
			((b) < (c) ? (b) : (c))
			);
	}

	template<class T>
	T Engine_ABS(const T& a)
	{
		return a >= 0.0f ? a : -a;
	}

	inline bool IsNAN(float val)
	{
		return val != val;
	}

	inline bool IsNAN(double val)
	{
		return val != val;
	}
}