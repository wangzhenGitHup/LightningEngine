#include "Intersection.h"
#include "Ray.h"
#include "Sphere.h"
#include "AABB.h"
#include "Plane.h"
#include "Math/Vector3.h"

namespace LightningEngine
{
	bool IntersectRaySphere(Ray & ray, const Sphere & sphere)
	{
		Vector3 ray_2_sphere_center = sphere.GetCenter() - ray.GetOrigin();
		float  dis = Dot(ray_2_sphere_center, ray.GetDirection()); //判断是在ray的前面还是后面
		float sqrDis = Dot(ray_2_sphere_center, ray_2_sphere_center);
		float sqrRadius = ENGINE_SQUARE(sphere.GetRadius()); 
		if (dis < 0.0f && sqrDis > sqrRadius)
		{
			return false;
		}

		float sqrMag = sqrDis - ENGINE_SQUARE(dis);
		return !(sqrMag > sqrRadius);
	}

	bool IntersectAABBFrustumFull(AABB & aabb, Plane plane[6])
	{
		const Vector3& vecCenter = aabb.GetCenter();
		const Vector3& vecExt = aabb.GetExtent();
		for (int i = 0; i < 6; i++, plane++)
		{
			const Vector3& vecNormal = plane->GetNormal();
			float dis = plane->GetDistanceToPoint(vecCenter);
			float radius = Dot(vecExt, Abs(vecNormal));
			if (dis + radius < 0)
			{
				return false;
			}
		}
		return true;
	}

	bool IntersectSphereFrustumFull(const Vector3 & center, float radius, const Plane plane[6])
	{
		for (int i = 0; i < 6; i++, plane++)
		{
			float dis = plane->GetDistanceToPoint(center);
			if (dis + radius < 0)
			{
				return false;
			}
		}
		return true;
	}
}

