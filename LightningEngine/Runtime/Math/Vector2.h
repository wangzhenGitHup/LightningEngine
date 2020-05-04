#pragma once

#include "Float.h"

namespace LightningEngine
{
	class Vector2
	{
	public:
		union
		{
			struct
			{
				float x_, y_;
			};
			float v_[2];
		};

	public:
		Vector2(float x, float y) : x_(x), y_(y) {}
		Vector2(const Vector2& v) : x_(v.x_), y_(v.y_) {}
		Vector2(float* arr) : x_(arr[0]), y_(arr[1]) {}
		Vector2() : x_(0.0f), y_(0.0f) {}

		void Set(const float& xVal, const float& yVal);
		void Set(const float* v);
		bool operator==(const Vector2& v);
		bool operator!=(const Vector2& v);
		void operator=(const Vector2& v);
		Vector2 operator+(const Vector2& v);
		void operator+=(const Vector2& v);
		Vector2 operator-(const Vector2& v);
		void operator-=(const Vector2& v);
		Vector2 operator*(float scalar);
		void operator*=(float scalar);
		float operator*(const Vector2& v);
		float& operator[](int idx) { return v_[idx]; }
		Vector2 operator/(float val);
		void operator/=(float val);
		float LengthSquared();
		float Length();
		bool IsPerpendicularTo(Vector2& v);
		Vector2 ProjectTo(Vector2& v);
		Vector2 PerpendicularTo(Vector2& v);
		float Sin(Vector2& v);
		float Cos(Vector2& v);
	};

	Vector2 operator*(float scalar, Vector2& v);
	Vector2 operator-(const Vector2& v);
	Vector2 Normalize(Vector2& v);
}