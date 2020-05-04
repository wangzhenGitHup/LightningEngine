#pragma once

namespace LightningEngine
{
	class Vector2;
	class Vector3;

	class Vector4
	{
	public:
		union
		{
			struct
			{
				float x_, y_, z_, w_;
			};

			float v_[4];
		};

	public:
		Vector4() : x_(0.0f), y_(0.0f), z_(0.0f), w_(1.0f) {}
		Vector4(float x, float y, float z, float w = 1.0f) : x_(x), y_(y), z_(z), w_(w) {}
		Vector4(const Vector4& v) : x_(v.x_), y_(v.y_), z_(v.z_), w_(v.w_) {}
		Vector4(const float* arr) : x_(arr[0]), y_(arr[1]), z_(arr[2]), w_(arr[3]) {}

		Vector4& operator=(const Vector4& v);
		Vector4& operator=(const Vector2& v);
		bool operator==(const Vector4& v)const;
		bool operator!=(const Vector4& v)const;
		bool operator==(const float arr[4])const;
		bool operator!=(const float arr[4])const;
		Vector4 operator*(float v);
		Vector4 operator+(const Vector4& v);
		Vector4 operator-(const Vector4& v);
		void Set(float x, float y, float z, float w = 1.0f);
		Vector4 operator-()const;
		float& operator[](int idx);
		float SqrtMagnitude();
		float Magnitude();
	};

	typedef Vector4 Color4;
	inline Color4 Lerp(Color4 from, Color4 to, float percent)
	{
		return Color4(from * percent + to * (1.0f - percent));
	}
}