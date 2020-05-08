#pragma once

#include <algorithm>
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Vector3.h"


namespace LightningEngine
{
	class Quaternion
	{
	public:
		Quaternion() {}
		Quaternion(float x, float y, float z, float w) : x_(x), y_(y), z_(z), w_(w) {}

		inline friend Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs)
		{
			return Quaternion(
				lhs.w_ * rhs.x_ + lhs.x_ * rhs.w_ + lhs.y_ * rhs.z_ - lhs.z_ * rhs.y_,
				lhs.w_ * rhs.y_ + lhs.y_ * rhs.w_ + lhs.z_ * rhs.x_ - lhs.x_ * rhs.z_,
				lhs.w_ * rhs.z_ + lhs.z_ * rhs.w_ + lhs.x_ * rhs.y_ - lhs.y_ * rhs.x_,
				lhs.w_ * rhs.w_ - lhs.x_ * rhs.x_ - lhs.y_ * lhs.y_ - lhs.z_ * rhs.z_
			);
		}

		void Set(float x, float y, float z, float w);

	
		float x_, y_, z_, w_;
	};


	Quaternion EulerToQuaternion(const Vector3& euler);
	void QuaternionToMatrix(const Quaternion& q, Matrix3x3& mat);
}