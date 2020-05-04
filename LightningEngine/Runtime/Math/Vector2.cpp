#include "Vector2.h"

namespace LightningEngine
{
	void Vector2::Set(const float & xVal, const float & yVal)
	{
		x_ = xVal;
		y_ = yVal;
	}

	void Vector2::Set(const float * v)
	{
		x_ = v[0];
		y_ = v[1];
	}

	bool Vector2::operator==(const Vector2 & v)
	{
		return x_ == v.x_ && y_ == v.y_;
	}

	bool Vector2::operator!=(const Vector2 & v)
	{
		return x_ != v.x_ || y_ != v.y_;
	}

	void Vector2::operator=(const Vector2 & v)
	{
		x_ = v.x_;
		y_ = v.y_;
	}

	Vector2 Vector2::operator+(const Vector2 & v)
	{
		return Vector2(x_ + v.x_, y_ + v.y_);
	}

	void Vector2::operator+=(const Vector2 & v)
	{
		x_ += v.x_;
		y_ += v.y_;
	}

	Vector2 Vector2::operator-(const Vector2 & v)
	{
		return Vector2(x_ - v.x_, y_ - v.y_);
	}

	void Vector2::operator-=(const Vector2 & v)
	{
		x_ -= v.x_;
		y_ -= v.y_;
	}

	Vector2 Vector2::operator*(float scalar)
	{
		return Vector2(x_ * scalar, y_ * scalar);
	}

	void Vector2::operator*=(float scalar)
	{
		x_ *= scalar;
		y_ *= scalar;
	}

	float Vector2::operator*(const Vector2 & v)
	{
		return x_ * v.x_ + y_ * v.y_;
	}
	Vector2 Vector2::operator/(float val)
	{
		if (!FLT_IS_ZERO(val))
		{
			float reciprocal = 1.0f / val;
			return Vector2(x_ * reciprocal, y_ * reciprocal);
		}

		return Vector2(x_, y_);
	}

	void Vector2::operator/=(float val)
	{
		if (!FLT_IS_ZERO(val))
		{
			float reciprocal = 1.0f / val;
			x_ *= reciprocal;
			y_ *= reciprocal;
		}
	}

	float Vector2::LengthSquared()
	{
		return ENGINE_SQUARE(x_) + ENGINE_SQUARE(y_);
	}

	float Vector2::Length()
	{
		float length = ENGINE_SQRTF(LengthSquared());
		return length;
	}

	bool Vector2::IsPerpendicularTo(Vector2 & v)
	{
		return FLT_IS_ZERO((*this)*v);
	}

	Vector2 Vector2::ProjectTo(Vector2 & v)
	{
		float lenSquared = v.LengthSquared();
		return (*this) * v * v / lenSquared;
	}

	Vector2 Vector2::PerpendicularTo(Vector2 & v)
	{
		Vector2 projP2Q = ProjectTo(v);
		return (*this) - projP2Q;
	}

	float Vector2::Sin(Vector2 & v)
	{
		float cos = Cos(v);
		return ENGINE_SQRTF(1 - ENGINE_SQUARE(cos));
	}

	float Vector2::Cos(Vector2 & v)
	{
		return ((*this) * v) / (Length() * v.Length());
	}

	Vector2 operator*(float scalar, Vector2 & v)
	{
		return Vector2(v.x_ * scalar, v.y_ * scalar);
	}

	Vector2 operator-(const Vector2 & v)
	{
		return Vector2();
	}

	Vector2 Normalize(Vector2 & v)
	{
		return v / v.Length();
	}
}