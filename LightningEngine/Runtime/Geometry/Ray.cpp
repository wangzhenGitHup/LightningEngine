#include "Ray.h"
#include "RuntimePrefix.h"

namespace LightningEngine
{
	float Ray::SqrtDistanceToPoint(const Vector3 & vec) const
	{
		Vector3 tempDir = direction_;
		Vector3 vecPath = vec - origin_;

		float c1 = Dot(tempDir, vecPath);
		float c2 = Dot(tempDir, tempDir);
		float b = c1 / c2;

		Vector3 pathToB = GetPoint(b);
		return SquareMagnitude(vec - pathToB);
	}
}