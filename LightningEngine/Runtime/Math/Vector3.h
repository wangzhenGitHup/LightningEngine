#pragma once

#include "Float.h"

#define VEC3_CROSS(dest, v1, v2) do{ \
	dest[0] = v1[1] * v2[2] - v1[2] * v2[2]; \
	dest[1] = v1[2] * v2[0] - v1[0] * v2[2]; \
	dest[2] = v1[0] * v2[1] - v1[1] * v2[0];}while(0)

#define VEC3_DOT(v1, v2) (v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2])

#define VEC3_SUB(dest, v1, v2) do{  \
	dest[0] = v1[0] - v2[0]; \
	dest[1] = v1[1] - v2[1]; \
	dest[2] = v1[2] - v2[2];}while(0)

namespace LightningEngine
{
	class Vector4;

	class Vector3
	{
	public:
		union
		{
			struct 
			{
				float x_, y_, z_;
			};
			float v_[3];
		};
		
	public:
		Vector3() : x_(0.0f), y_(0.0f), z_(0.0f) {}
		Vector3(float x, float y, float z) : x_(x), y_(y), z_(z) {}
		Vector3(const Vector3& v) : x_(v.x_), y_(v.y_), z_(v.z_) {}
		Vector3(const float* v) : x_(v[0]), y_(v[1]), z_(v[2]) {}

		Vector3& operator=(const Vector3& v);
		void operator=(const Vector4& v);
		Vector3 operator*(const float scalar);
		float operator*(const Vector3& v);
		Vector3 operator^(const Vector3& v);
		float& operator[](int idx);
		bool operator==(const Vector3& v);
		bool operator!=(const Vector3& v);
		Vector3& operator+=(const Vector3& v);
		Vector3& operator-=(const Vector3& v);
		Vector3& operator*=(float scalar);
		Vector3 operator-();
		Vector3 operator/(float scalar);
		Vector3& operator/=(float scalar);
		void Set(float x, float y, float z);
		void Set(const float* arr);
		float Length();
		float LengthSquared();
		Vector3 Normalize();
		bool IsPerpendicularTo(Vector3& v);
		Vector3 ProjectTo(Vector3& v);
		Vector3 PerpendicularTo(Vector3& v);
		float Sin(Vector3& v);
		float Cos(Vector3& v);
		static const float epsilon;
	};

	Vector3 operator+(const Vector3& lhs, const Vector3& rhs);
	Vector3 operator-(const Vector3& lhs, const Vector3& rhs);
	Vector3 operator*(const float scalar, const Vector3& v);
	Vector3 minVector3(const Vector3& lhs, const Vector3& rhs);
	Vector3 maxVector3(const Vector3& lhs, const Vector3& rhs);
	Vector3 Lerp(Vector3& from, Vector3& to, float t);
	Vector3 Abs(const Vector3& v);
	Vector3 Cross(const Vector3& lhs, const Vector3& rhs);
	float Dot(const Vector3& lhs, const Vector3& rhs);
	float SquareMagnitude(const Vector3& v);
	float Magnitude(const Vector3& v);
	Vector3 Normalize(Vector3& v);
	bool IsFinite(const Vector3& v);

}