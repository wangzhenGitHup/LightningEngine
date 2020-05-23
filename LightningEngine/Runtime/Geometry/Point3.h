#pragma once

#include "Math/Vector3.h"

namespace LightningEngine
{
	class Point3
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

		Point3() : x_(0.0f), y_(0.0f), z_(0.0f) {}
		Point3(float x, float y, float z) : x_(x), y_(y), z_(z) {}

		Point3 operator+(const Vector3& vec)
		{
			return Point3(x_ + vec.v_[0], y_ + vec.v_[1], z_ + vec.v_[2]);
		}

		void operator+=(const Vector3& vec)
		{
			x_ += vec.v_[0];
			y_ += vec.v_[1];
			z_ += vec.v_[2];
		}

		Vector3 operator-(const Point3& p)
		{
			return Vector3(x_ - p.v_[0], y_ - p.v_[1], z_ - p.v_[2]);
		}

		Point3 operator-(const Vector3& vec)
		{
			return Point3(x_ - vec.v_[0], y_ - vec.v_[1], z_ - vec.v_[2]);
		}

		void operator-=(const Vector3& vec)
		{
			x_ -= vec.v_[0];
			y_ -= vec.v_[1];
			z_ -= vec.v_[2];
		}
	};
}