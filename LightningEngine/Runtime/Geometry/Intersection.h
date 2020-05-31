#pragma once

namespace LightningEngine
{
	class Ray;
	class AABB;
	class Sphere;
	class Vector3;
	class Plane;

	bool IntersectRaySphere(Ray& ray, const Sphere& sphere);
	bool IntersectAABBFrustumFull(AABB& aabb, Plane plane[6]);
	bool IntersectSphereFrustumFull(const Vector3& center, float radius, const Plane plane[6]);
}