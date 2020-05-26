#include "Sphere.h"
#include "AABB.h"
#include "Render/VertexData.h"

namespace LightningEngine
{
	void Sphere::Set(const Vector3 & vec1, const Vector3 & vec2)
	{
		Vector3 vecHalf = (vec2 - vec1) * 0.5f;
		center_ = vecHalf + vec1;
		radius_ = Magnitude(vecHalf);
	}

	void Sphere::Set(const Vector3 * pVec, UInt32 cnt)
	{
		radius_ = 0.0f;
		center_ = Vector3(0.0f, 0.0f, 0.0f);
		for (UInt32 idx = 0; idx < cnt; idx++)
		{
			radius_ = std::max<float>(radius_, SquareMagnitude(pVec[idx]));
		}
		radius_ = sqrtf(radius_);
	}

	void Sphere::Set(VertexDataFull * pData, UInt32 cnt)
	{
		radius_ = 0.0f;
		center_ = Vector3(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < cnt; i++)
		{
			radius_ = std::max<float>(radius_, pData[i].vertex_.SqrtMagnitude());
		}
		radius_ = sqrtf(radius_);
	}

	void Sphere::Set(const MinMaxAABB & aabb)
	{
		center_ = Vector3((aabb.max_.x_ + aabb.min_.x_) / 2.0f, 
			(aabb.max_.y_ + aabb.min_.y_) / 2.0f, 
			(aabb.max_.z_ + aabb.min_.z_) / 2.0f);
		float deltaX = aabb.max_.x_ - center_.x_;
		float deltaY = aabb.max_.y_ - center_.y_;
		float deltaZ = aabb.max_.z_ - center_.z_;
		radius_ = sqrtf(ENGINE_SQUARE(deltaX) + ENGINE_SQUARE(deltaY) + ENGINE_SQUARE(deltaZ));
	}

	bool Sphere::IsInside(const Sphere & sphere) const
	{
		float fSqrDist = SquareMagnitude(GetCenter() - sphere.GetCenter());
		if (ENGINE_SQUARE(GetRadius()) > fSqrDist + ENGINE_SQUARE(sphere.GetRadius()))
		{
			return true;
		}

		return false;
	}

	float CalculateSqrDistance(const Vector3 & vec, const Sphere & sphere)
	{
		return ENGINE_MAX(0.0f, 
			SquareMagnitude(vec - sphere.GetCenter()) - ENGINE_SQUARE(sphere.GetRadius()));
	}

	bool Intersect(const Sphere & sphere1, const Sphere & sphere2)
	{
		//计算两圆心的距离平方
		float fSqrDist = SquareMagnitude(sphere1.GetCenter() - sphere2.GetCenter());

		if (ENGINE_SQUARE(sphere1.GetRadius() + sphere2.GetRadius()) > fSqrDist)
		{
			return true;
		}

		return false;
	}
}