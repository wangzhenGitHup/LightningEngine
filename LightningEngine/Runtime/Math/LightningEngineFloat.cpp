#include "LightningEngineFloat.h"

bool IsFinite(const float& val)
{
	UInt32 intVal = *reinterpret_cast<const UInt32*>(&val);
	return (intVal & 0x7f800000) != 0x7f800000;
}

bool IsFinite(const double& val)
{
	UInt64 intval = *reinterpret_cast<const UInt64*>(&val);
	return (intval & 0x7ff0000000000000LL) != 0x7ff0000000000000LL;
}
namespace LightningEngine
{
	int FllorfToInt(float f)
	{
		return f >= 0 ? (int)f : (int)(f - sBiggestFloatSmallerThanOne);
	}

	bool EqualApproximately(float f0, float f1, float epsilon)
	{
		float dist = (f0 - f1);
		dist = ENGINE_ABS(dist);
		return dist <= epsilon;
	}

	float CopySignf(float x, float y)
	{
		union 
		{
			float fval;
			UInt32 ival;
		}u, u0, u1;
		u0.fval = x;
		u1.fval = y;
		UInt32 a = u0.ival;
		UInt32 b = u1.ival;
		Int32 mask = 1 << 31;
		UInt32 sign = b & mask;

		a &= ~mask;
		a |= sign;
		u.ival = a;

		return u.fval;
	}
	float LinearInterpolate(float from, float to, float t)
	{
		return to * t + from * (1.0f - t);
	}
}