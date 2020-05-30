#pragma once

#include "Math/Vector2.h"

namespace LightningEngine
{
	class CubicBezier
	{
	public:
		void Init(float x1, float y1, float x2, float y2);
		float GetXValueFromPercent(float percent);
		float GetYValueFromPercent(float percent);

	public:
		Vector2 vecPoint1_, vecPoint2_;
		float ax_, bx_, cx_, ay_, by_, cy_;
	};
}