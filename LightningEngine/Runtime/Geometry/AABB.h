#pragma once

#include "Base/Object.h"
#include "Math/Vector3.h"
#include "Math/Matrix4x4.h"
#include "Math/Quaternion.h"

namespace LightningEngine
{
	class MinMaxAABB;
	class Matrix3x3;

	class AABB
	{
	public:
		AABB() {}
		AABB(const Vector3& center, const Vector3& extent) : center_(center), extent_(extent){}
		AABB(const MinMaxAABB& aabb) { FromMinMaxAABB(aabb); }
		AABB& operator=(const MinMaxAABB& aabb) { FromMinMaxAABB(aabb); return *this; }
		bool operator==(const AABB& aabb) { return center_ == aabb.center_ && extent_ == aabb.extent_; }
		void SetCenterAndExtent(const Vector3& center, const Vector3& extent) { center_ = center; extent_ = extent; }
		Vector3& GetCenter() { return center_; }
		Vector3& GetExtent() { return extent_; }
		float& GetExtent(int i) { return extent_[i]; }
		Vector3 GetMin()const { return center_ - extent_; }
		Vector3 GetMax()const { return center_ + extent_; }
		void Expand(float val);
		bool IsValid()const;
		bool IsInside(Vector3& vec);
		void GetVertices(Vector3* outVertices)const;
		void Encapsulate(const Vector3& vec);

	public:
		static const AABB s_zero;
		Vector3 center_;
		Vector3 extent_;

	private:
		void FromMinMaxAABB(const MinMaxAABB& aabb);
	};

	MinMaxAABB AddAABB(MinMaxAABB& lhs, MinMaxAABB& rhs);
	void TransformAABB(AABB& aabb, Vector3& pos, Quaternion& rotation, AABB& ret);
	void TransformAABB(AABB& aabb, Matrix4x4& transform, AABB& ret);
	float CalculateSqrDistance(Vector3& rkPos, AABB& rkBox);
	void CalculateClosestPoint(Vector3& rkPos, AABB& rkBox, Vector3& outPos, float& outSqrDistance);
	Vector3 RotateExtents(const Vector3& vecExt, const Matrix3x3& matRotate);
	Vector3 RotateExtents(const Vector3& vecExt, const Matrix4x4& matRotate);

	/*************************MinMaxAABB*******************************/
	class MinMaxAABB
	{
	public:
		MinMaxAABB() { Init(); }
		MinMaxAABB(Vector3 minVec, Vector3 maxVec) : min_(minVec), max_(maxVec) {}
		MinMaxAABB(AABB& aabb) { FromAABB(aabb); }
		void Init();
		const Vector3& GetMin()const { return min_; }
		const Vector3& GetMax()const { return max_; }
		Vector3 GetCenter()const { return 0.5f * (max_ + min_); }
		Vector3 GetExtent()const { return 0.5f * (max_ - min_); }
		Vector3 GetSize()const { return max_ - min_; }
		void Encapsulate(const Vector3& vec);
		void Encapsulate(const Vector4& vec);
		void Encapsulate(AABB& aabb);
		void Encapsulate(const MinMaxAABB& other);
		void Expand(float val);
		void Expand(const Vector3& vecOffset);
		bool IsVaild();
		bool IsInside(Vector3& vecPos);
		void GetVertices(Vector3 outVertices[8])const;

	private:
		void FromAABB(AABB& aabb);

	public:
		Vector3 min_;
		Vector3 max_;
	};

	inline void AABB::Expand(float val)
	{
		extent_ += Vector3(val, val, val);
	}

	inline void AABB::FromMinMaxAABB(const MinMaxAABB& aabb)
	{
		center_ = (aabb.GetMax() + aabb.GetMin()) * 0.5f;
		extent_ = (aabb.GetMax() - aabb.GetMin()) * 0.5f;
	}

	inline bool AABB::IsValid()const
	{
		return IsFinite(center_) && IsFinite(extent_);
	}

	inline void MinMaxAABB::Encapsulate(AABB& aabb)
	{
		Encapsulate(aabb.GetCenter() + aabb.GetExtent());
		Encapsulate(aabb.GetCenter() - aabb.GetExtent());
	}

	inline void MinMaxAABB::Encapsulate(const MinMaxAABB& other)
	{
		min_ = minVector3(min_, other.min_);
		max_ = maxVector3(max_, other.max_);
	}

	inline void MinMaxAABB::Expand(float val)
	{
		Vector3 vecOffset = Vector3(val, val, val);
		min_ -= vecOffset;
		max_ += vecOffset;
	}

	inline void MinMaxAABB::Expand(const Vector3& vecOffset)
	{
		min_ -= vecOffset;
		max_ += vecOffset;
	}

	inline void MinMaxAABB::FromAABB(AABB& aabb)
	{
		min_ = aabb.GetCenter() - aabb.GetExtent();
		max_ = aabb.GetCenter() + aabb.GetExtent();
	}
}