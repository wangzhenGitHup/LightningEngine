#pragma once

#include "Math/Vector3.h"

namespace LightningEngine
{

	class Bezier
	{
	public:
		Bezier() {}
		Bezier(Bezier& other);
		void Init(float x1, float y1, float z1,
			float x2, float y2, float z2,
			float x3, float y3, float z3,
			float x4, float y4, float z4);
		Vector3 GetPointOnBezier(float val);
		float GetYValueWithX(float x);
		float GetXValueWithY(float y);

	protected:
		void InitDValueWithX(float x);
		void InitDValueWithY(float y);
		float GetTValue();

	protected:
		Vector3 vecPoints_[4];
		float a_, b_, c_, d_, A_, B_, C_;
	};
}