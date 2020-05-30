#include "CubicBzeier.h"

namespace LightningEngine
{
	void CubicBezier::Init(float x1, float y1, float x2, float y2)
	{
		vecPoint1_.Set(x1, y1);
		vecPoint2_.Set(x2, y2);

		ax_ = 3 * x1 - 3 * x2 + 1;
		bx_ = 3 * x2 - 6 * x1;
		cx_ = 3 * x1;

		ay_ = 3 * y1 - 3 * y2 + 1;
		by_ = 3 * y2 - 6 * y1;
		cy_ = 3 * y1;
	}

	float CubicBezier::GetXValueFromPercent(float percent)
	{
		return ((ax_ * percent + bx_) * percent + cx_) * percent;
	}

	float CubicBezier::GetYValueFromPercent(float percent)
	{
		return ((ay_ * percent + by_) * percent + cy_) * percent;
	}
}