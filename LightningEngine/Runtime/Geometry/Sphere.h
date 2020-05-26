#pragma once

#include "Math/Vector3.h"
#include <algorithm>

namespace LightningEngine
{
	class MinMaxAABB;
	struct VertexDataFull;

	class Sphere
	{
	public:
		Sphere() : radius_(0.0f) {}
		Sphere(const Vector3& vec, float r) {}
		void Set(const Vector3& vec) { center_ = vec, radius_ = 0.0f; }
		void Set(const Vector3& vec, float r) { center_ = vec, radius_ = r; }
		void Set(const Vector3& vec1, const Vector3& vec2);
		void Set(const Vector3* pVec, UInt32 cnt);
		void Set(VertexDataFull* pData, UInt32 cnt);
		void Set(const MinMaxAABB& aabb);
		Vector3& GetCenter() { return center_; }
		const Vector3& GetCenter()const { return center_; }
		float& GetRadius() { return radius_; }
		const float& GetRadius()const { return radius_; }
		bool IsInside(const Sphere& sphere)const;

	public:
		float radius_;
		Vector3 center_;
	};

	float CalculateSqrDistance(const Vector3& vec, const Sphere& sphere);

	//¼ì²âÁ½Ô²µÄ½»»¥
	bool Intersect(const Sphere& sphere1, const Sphere& sphere2);

}