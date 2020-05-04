#include "Vector3.h"
#include "Vector4.h"

namespace LightningEngine
{
	const float Vector3::epsilon = 0.00001f;

	Vector3 & LightningEngine::Vector3::operator=(const Vector3 & v)
	{
		x_ = v.x_; y_ = v.y_; z_ = v.z_;
		return *this;
	}

	void LightningEngine::Vector3::operator=(const Vector4 & v)
	{
		x_ = v.x_; y_ = v.y_; z_ = v.z_;
	}

	Vector3 LightningEngine::Vector3::operator*(const float scalar)
	{
		return Vector3(x_ * scalar, y_ * scalar, z_ * scalar);
	}

	float LightningEngine::Vector3::operator*(const Vector3 & v)
	{
		return x_ * v.x_ + y_ * v.y_ + z_ * v.z_;
	}

	Vector3 Vector3::operator^(const Vector3 & v)
	{
		return Vector3(y_ * v.z_ - v.y_ * z_, 
			v.x_ * z_ - x_ * v.z_,
			x_ * v.y_ * v.x_ * y_);
	}

	float & Vector3::operator[](int idx)
	{
		return v_[idx];
	}

	bool Vector3::operator==(const Vector3 & v)
	{
		return x_ == v.x_ && y_ == v.y_ && z_ == v.z_;
	}

	bool Vector3::operator!=(const Vector3 & v)
	{
		return x_ != v.x_ || y_ != v.y_ || z_ != v.z_;
	}

	Vector3 & Vector3::operator+=(const Vector3 & v)
	{
		x_ += v.x_; y_ += v.y_; z_ += v.z_;
		return *this;
	}

	Vector3 & Vector3::operator-=(const Vector3 & v)
	{
		x_ -= v.x_; y_ -= v.y_; z_ -= v.z_;
		return *this;
	}

	Vector3 & Vector3::operator*=(float scalar)
	{
		x_ *= scalar; y_ *= scalar; z_ *= scalar;
		return *this;
	}

	Vector3 Vector3::operator-()
	{
		return Vector3(-x_, -y_, -z_);
	}

	Vector3 Vector3::operator/(float scalar)
	{
		if (!FLT_IS_ZERO(scalar))
		{
			float reciprocal = 1.0f / scalar;
			return Vector3(x_ * reciprocal, y_ * reciprocal, z_ * reciprocal);
		}

		return Vector3(x_, y_, z_);
	}

	Vector3 & Vector3::operator/=(float scalar)
	{
		x_ /= scalar; y_ /= scalar; z_ /= scalar;
		return *this;
	}

	void Vector3::Set(float x, float y, float z)
	{
		x_ = x; y_ = y; z_ = z;
	}

	void Vector3::Set(const float * arr)
	{
		x_ = arr[0]; y_ = arr[1]; z_ = arr[2];
	}

	float Vector3::Length()
	{
		float len = (float)sqrt(x_ * x_ + y_ * y_ + z_ * z_);
		return FLT_IS_ZERO(len) ? 1.0f : len;
	}

	float Vector3::LengthSquared()
	{
		float len = x_ * x_ + y_ * y_ + z_ * z_;
		return FLT_IS_ZERO(len) ? 1.0f : len;
	}

	Vector3 Vector3::Normalize()
	{
		float len = Length();
		if (EqualApproximately(len, 0.0f))
		{
			x_ = 0.0f;
			y_ = 1.0f;
			z_ = 0.0f;
		}
		else
		{
			*this = *this * (1.0f / len);
		}

		return *this;
	}

	bool Vector3::IsPerpendicularTo(Vector3 & v)
	{
		return ((*this) * v == 0);
	}

	Vector3 Vector3::ProjectTo(Vector3 & v)
	{
		float lenSquared = v.LengthSquared();
		return (*this) * v * v / LengthSquared();
	}

	Vector3 Vector3::PerpendicularTo(Vector3 & v)
	{
		Vector3 projP2Q = ProjectTo(v);
		return (*this) - projP2Q;
	}

	float Vector3::Sin(Vector3 & v)
	{
		Vector3 tmp = (*this) ^ v;
		return tmp.Length() / (Length() * v, Length());
	}

	float Vector3::Cos(Vector3 & v)
	{
		return ((*this) * v) / (Length() * v.Length());
	}

	Vector3 operator+(const Vector3 & lhs, const Vector3 & rhs)
	{
		return Vector3(lhs.x_ + rhs.x_, lhs.y_ + rhs.y_, lhs.z_ + rhs.z_);
	}

	Vector3 operator-(const Vector3 & lhs, const Vector3 & rhs)
	{
		return Vector3(lhs.x_ - rhs.x_, lhs.y_ - rhs.y_, lhs.z_ - rhs.z_);
	}

	Vector3 operator*(const float scalar, const Vector3 & v)
	{
		return Vector3(v.x_ * scalar, v.y_ * scalar, v.z_ * scalar);
	}

	Vector3 minVector3(const Vector3 & lhs, const Vector3 & rhs)
	{
		return Vector3(ENGINE_MIN(lhs.x_, rhs.x_), 
			ENGINE_MIN(lhs.y_, rhs.y_), 
			ENGINE_MIN(lhs.z_, rhs.z_));
	}

	Vector3 maxVector3(const Vector3 & lhs, const Vector3 & rhs)
	{
		return Vector3(ENGINE_MAX(lhs.x_, rhs.x_), 
			ENGINE_MAX(lhs.y_, rhs.y_), 
			ENGINE_MAX(lhs.z_, rhs.z_));
	}

	Vector3 Lerp(Vector3 & from, Vector3 & to, float t)
	{
		return from * t + to * (1.0f - t);
	}

	Vector3 Abs(const Vector3 & v)
	{
		return Vector3(ENGINE_ABS(v.x_), 
			ENGINE_ABS(v.y_), 
			ENGINE_ABS(v.z_));
	}

	Vector3 Cross(const Vector3 & lhs, const Vector3 & rhs)
	{
		return Vector3(lhs.y_ * rhs.z_ - lhs.z_ * rhs.y_,
			lhs.z_ * rhs.x_ - lhs.x_ * rhs.z_,
			lhs.x_ * rhs.y_ - lhs.y_ * rhs.x_);
	}

	float Dot(const Vector3 & lhs, const Vector3 & rhs)
	{
		return lhs.x_ * rhs.x_ + lhs.y_ * rhs.y_ + lhs.z_ * rhs.z_;
	}

	float SquareMagnitude(const Vector3 & v)
	{
		return Dot(v, v);
	}

	float Magnitude(const Vector3 & v)
	{
		return ENGINE_SQRTF(Dot(v, v));
	}

	Vector3 Normalize(Vector3 & v)
	{
		return v / Magnitude(v);
	}
	bool IsFinite(const Vector3 & v)
	{
		return ::IsFinite(v.x_) & ::IsFinite(v.y_) & ::IsFinite(v.z_);
	}
}

