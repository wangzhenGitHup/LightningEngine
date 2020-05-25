#include "AABB.h"
#include "RuntimePrefix.h"
#include "Math/Matrix3x3.h"
#include "Math/Quaternion.h"

namespace LightningEngine
{
	const AABB AABB::s_zero = AABB(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));

	bool AABB::IsInside(Vector3 & vec)
	{
		if (vec[0] < center_[0] - extent_[0])
		{
			return false;
		}

		if (vec[0] > center_[0] + extent_[0])
		{
			return false;
		}

		if (vec[1] < center_[1] - extent_[1])
		{
			return false;
		}

		if (vec[1] > center_[1] + extent_[1])
		{
			return false;
		}

		if (vec[2] < center_[2] - extent_[2])
		{
			return false;
		}

		if (vec[2] > center_[2] + extent_[2])
		{
			return false;
		}

		return true;
	}

	void AABB::GetVertices(Vector3 * outVertices) const
	{
		outVertices[0] = center_ + Vector3(-extent_.x_, -extent_.y_, -extent_.z_);
		outVertices[1] = center_ + Vector3(extent_.x_, -extent_.y_, -extent_.z_);
		outVertices[2] = center_ + Vector3(-extent_.x_, extent_.y_, -extent_.z_);
		outVertices[3] = center_ + Vector3(extent_.x_, extent_.y_, -extent_.z_);
		outVertices[4] = center_ + Vector3(-extent_.x_, -extent_.y_, extent_.z_);
		outVertices[5] = center_ + Vector3(extent_.x_, -extent_.y_, extent_.z_);
		outVertices[6] = center_ + Vector3(extent_.x_, extent_.y_, extent_.z_);
		outVertices[7] = center_ + Vector3(extent_.x_, extent_.y_, extent_.z_);
	}

	void AABB::Encapsulate(const Vector3 & vec)
	{
		MinMaxAABB aabb = *this;
		aabb.Encapsulate(vec);
		FromMinMaxAABB(aabb);
	}

	void MinMaxAABB::Init()
	{
		min_ = Vector3(std::numeric_limits<float>::infinity(),
			std::numeric_limits<float>::infinity(),
			std::numeric_limits<float>::infinity());

		max_ = -Vector3(std::numeric_limits<float>::infinity(),
			std::numeric_limits<float>::infinity(),
			std::numeric_limits<float>::infinity());
	}

	MinMaxAABB AddAABB(MinMaxAABB & lhs, MinMaxAABB & rhs)
	{
		MinMaxAABB aabb;
		if (lhs.IsVaild())
		{
			aabb = lhs;
		}

		if (rhs.IsVaild())
		{
			aabb.Encapsulate(rhs.GetMax());
			aabb.Encapsulate(rhs.GetMin());
		}

		return aabb;
	}

	bool MinMaxAABB::IsInside(Vector3 & vecPos)
	{
		if (vecPos[0] < min_[0])
		{
			return false;
		}

		if (vecPos[0] > max_[0])
		{
			return false;
		}

		if (vecPos[1] < min_[1])
		{
			return false;
		}

		if (vecPos[1] > max_[1])
		{
			return false;
		}

		if (vecPos[2] < min_[2])
		{
			return false;
		}

		if (vecPos[2] > max_[2])
		{
			return false;
		}

		return true;
	}

	bool MinMaxAABB::IsVaild()
	{
		return !(min_ == Vector3(std::numeric_limits<float>::infinity(),
			std::numeric_limits<float>::infinity(),
			std::numeric_limits<float>::infinity()) ||
			max_ == -Vector3(std::numeric_limits<float>::infinity(),
				std::numeric_limits<float>::infinity(),
				std::numeric_limits<float>::infinity()));
	}

	void MinMaxAABB::Encapsulate(const Vector4& vecPos)
	{
		min_.x_ = ENGINE_MIN(min_.x_, vecPos.x_);
		min_.y_ = ENGINE_MIN(min_.y_, vecPos.y_);
		min_.z_ = ENGINE_MIN(min_.z_, vecPos.z_);

		max_.x_ = ENGINE_MAX(max_.x_, vecPos.x_);
		max_.y_ = ENGINE_MAX(max_.y_, vecPos.y_);
		max_.z_ = ENGINE_MAX(max_.z_, vecPos.z_);
	}

	void MinMaxAABB::Encapsulate(const Vector3& vecPos)
	{
		min_.x_ = ENGINE_MIN(min_.x_, vecPos.x_);
		min_.y_ = ENGINE_MIN(min_.y_, vecPos.y_);
		min_.z_ = ENGINE_MIN(min_.z_, vecPos.z_);

		max_.x_ = ENGINE_MAX(max_.x_, vecPos.x_);
		max_.y_ = ENGINE_MAX(max_.y_, vecPos.y_);
		max_.z_ = ENGINE_MAX(max_.z_, vecPos.z_);
	}

	void TransformAABB(AABB & aabb, Vector3 & pos, Quaternion & rotation, AABB & ret)
	{
		Matrix3x3 mat;
		QuaternionToMatrix(rotation, mat);

		Vector3 vecExt = RotateExtents(aabb.GetExtent(), mat);
		Vector3 vecCenter = mat.MultiplyPoint3(aabb.GetCenter());
		vecCenter += pos;

		ret.SetCenterAndExtent(vecCenter, vecExt);
	}

	void TransformAABB(AABB & aabb, Matrix4x4 & transform, AABB & ret)
	{
		Vector3 vecExt = RotateExtents(aabb.GetExtent(), transform);
		Vector3 vecCenter = transform.MultiplyPoint3(aabb.GetCenter());
		ret.SetCenterAndExtent(vecCenter, vecExt);
	}

	float CalculateSqrDistance(Vector3 & rkPos, AABB & rkBox)
	{
		Vector3 vecClosest = rkPos - rkBox.GetCenter();
		float fSqrDistance = 0.0f;

		for (int i = 0; i < 3; i++)
		{
			float fCloseVal = vecClosest[i];
			float fExtVal = rkBox.GetExtent(i);
			if (fCloseVal < -fExtVal)
			{
				float delta = fCloseVal + fExtVal;
				fSqrDistance += delta * delta;
				vecClosest[i] = -fExtVal;
			}
			else if (fCloseVal > fExtVal)
			{
				float delta = fCloseVal - fExtVal;
				fSqrDistance += delta * delta;
			}
			vecClosest[i] = fExtVal;
		}

		return fSqrDistance;
	}

	void CalculateClosestPoint(Vector3 & rkPos, AABB & rkBox, 
		Vector3 & outPos, float & outSqrDistance)
	{
		Vector3 vecClosest = rkPos - rkBox.GetCenter();
		float fSqrDistance = 0.0f;
		float fDelta;
		
		for (int i = 0; i < 3; i++)
		{
			if (vecClosest[i] < -rkBox.GetExtent(i))
			{
				fDelta = vecClosest[i] + rkBox.GetExtent(i);
				fSqrDistance += fDelta * fDelta;
				vecClosest[i] = -rkBox.GetExtent(i);
			}
			else if (vecClosest[i] > rkBox.GetExtent(i))
			{
				fDelta = vecClosest[i] - rkBox.GetExtent(i);
				fSqrDistance += fDelta * fDelta;
				vecClosest[i] = rkBox.GetExtent(i);
			}
		}

		if (FLT_IS_ZERO(fSqrDistance))
		{
			outPos = rkPos;
			outSqrDistance = 0.0f;
		}
		else
		{
			outPos = vecClosest + rkBox.GetCenter();
			outSqrDistance = fSqrDistance;
		}
	}

	Vector3 RotateExtents(const Vector3 & vecExt, const Matrix3x3 & matRotate)
	{
		Vector3 retVec;
		for (int i = 0; i < 3; i++)
		{
			retVec[i] = ENGINE_ABS(matRotate.Get(i, 0) * vecExt.x_) +
				ENGINE_ABS(matRotate.Get(i, 1) * vecExt.y_) + 
				ENGINE_ABS(matRotate.Get(i, 2) * vecExt.z_);
		}
		return retVec;
	}

	Vector3 RotateExtents(const Vector3 & vecExt, const Matrix4x4 & matRotate)
	{
		Vector3 retVec;
		for (int i = 0; i < 3; i++)
		{
			retVec[i] = ENGINE_ABS(matRotate.Get(i, 0) * vecExt.x_) +
				ENGINE_ABS(matRotate.Get(i, 1) * vecExt.y_) +
				ENGINE_ABS(matRotate.Get(i, 2) * vecExt.z_);
		}
		return retVec;
	}
}