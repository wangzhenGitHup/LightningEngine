#include "Plane.h"

namespace LightningEngine
{
	Plane::Plane(float a, float b, float c, float d) : distance_(d)
	{
		normal_.x_ = a;
		normal_.y_ = b;
		normal_.z_ = c;
	}

	Plane & Plane::operator*=(float fScale)
	{
		normal_ *= fScale;
		distance_ *= fScale;
		return *this;
	}

	bool Plane::operator==(const Plane & plane)
	{
		return (normal_ == plane.normal_) && FLT_EQUAL(distance_, plane.distance_);
	}

	bool Plane::operator!=(const Plane & plane)
	{
		return (normal_ != plane.normal_) || !FLT_EQUAL(distance_, plane.distance_);
	}

	void Plane::SetInvalid()
	{
		normal_ = Vector3(0.0f, 0.0f, 0.0f);
		distance_ = 0.0f;
	}

	void Plane::SetABCD(const float a, const float b, const float c, const float d)
	{
		normal_.Set(a, b, c);
		distance_ = d;
	}

	void Plane::Normalize()
	{
		float invMag = 1.0f / Magnitude(normal_);
		normal_ *= invMag;
		distance_ *= invMag;
	}

	void Plane::SetNormalAndPosition(const Vector3 & normal, const Vector3 & point)
	{
		normal_ = normal;
		distance_ = -Dot(normal, point);
	}

	float Plane::GetDistanceToPoint(const Vector3 & point) const
	{
		return Dot(normal_, point) + distance_;
	}

	bool Plane::GetSide(const Vector3 & point) const
	{
		return Dot(normal_, point) + distance_ > 0.0f; //大于0为正面
	}
}