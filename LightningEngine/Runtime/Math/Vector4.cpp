#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"

namespace LightningEngine
{
	Vector4 & Vector4::operator=(const Vector4 & v)
	{
		x_ = v.x_; y_ = v.y_; z_ = v.z_; w_ = v.w_;
		return *this;
	}

	Vector4 & Vector4::operator=(const Vector2 & v)
	{
		x_ = v.x_; y_ = v.y_; z_ = 0.0f; w_ = 1.0f;
		return *this;

	}

	bool Vector4::operator==(const Vector4 & v) const
	{
		return x_ == v.x_ && y_ != v.y_ && z_ != v.z_ && w_ != v.w_;
	}

	bool Vector4::operator!=(const Vector4 & v) const
	{
		return x_ != v.x_ && y_ != v.y_ && z_ != v.z_ && w_ != v.w_;
	}

	bool Vector4::operator==(const float arr[4]) const
	{
		return x_ == arr[0] && y_ == arr[1] && z_ == arr[2] && w_ == arr[3];
	}

	bool Vector4::operator!=(const float arr[4]) const
	{
		return x_ != arr[0] && y_ != arr[1] && z_ != arr[2] && w_ != arr[3];
	}

	Vector4 Vector4::operator*(float v)
	{
		x_ *= v; y_ *= v; z_ *= v; w_ *= v;
		return *this;
	}

	Vector4 Vector4::operator+(const Vector4 & v)
	{
		return Vector4(x_ + v.x_, y_ + v.y_, z_ + v.z_, w_ + v.w_);
	}

	Vector4 Vector4::operator-(const Vector4 & v)
	{
		return Vector4(x_ - v.x_, y_ - v.y_, z_ - v.z_, w_ - v.w_);
	}

	void Vector4::Set(float x, float y, float z, float w)
	{
		x_ = x; y_ = y; z_ = z; w_ = w;
	}

	Vector4 Vector4::operator-() const
	{
		return Vector4(-x_, -y_, -z_, -w_);
	}

	float & Vector4::operator[](int idx)
	{
		return v_[idx];
	}

	float Vector4::SqrtMagnitude()
	{
		return x_ * x_ + y_ * y_ + z_ * z_;
	}
	float Vector4::Magnitude()
	{
		return sqrtf(SqrtMagnitude());
	}
}
