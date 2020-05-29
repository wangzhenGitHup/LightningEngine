#include "Bezier.h"

namespace LightningEngine
{
	Bezier::Bezier(Bezier & other)
	{
		memcpy(this, &other, sizeof(Bezier));
	}

	void Bezier::Init(float x1, float y1, float z1, 
		float x2, float y2, float z2, 
		float x3, float y3, float z3, 
		float x4, float y4, float z4)
	{
		vecPoints_[0].Set(x1, y1, z1);
		vecPoints_[1].Set(x2, y2, z2);
		vecPoints_[2].Set(x3, y3, z3);
		vecPoints_[3].Set(x4, y4, z4);
		a_ = 3 * x2 - x1 - 3 * x3 + x4;
		b_ = 3 * (x1 - 2 * x2 + x3);
		c_ = 3 * (x2 - x1);
	}

	Vector3 Bezier::GetPointOnBezier(float val)
	{
		float t = (val - vecPoints_[0].x_) / (vecPoints_[3].x_ - vecPoints_[0].x_);
		float x = vecPoints_[0].x_ * (1 - t) * (1 - t) * (1 - t) +
			3 * vecPoints_[1].x_ * t * (1 - t) * (1 - t) +
			3 * vecPoints_[2].x_ * t * t * (1 - t) +
			vecPoints_[3].x_ * t * t * t;

		float y = vecPoints_[0].y_ * (1 - t) * (1 - t) * (1 - t) +
			3 * vecPoints_[1].y_ * t * (1 - t) * (1 - t) +
			3 * vecPoints_[2].y_ * t * t * (1 - t) +
			vecPoints_[3].y_ * t * t * t;

		float z = vecPoints_[0].z_ * (1 - t) * (1 - t) * (1 - t) +
			3 * vecPoints_[1].z_ * t * (1 - t) * (1 - t) +
			3 * vecPoints_[2].z_ * t * t * (1 - t) +
			vecPoints_[3].z_ * t * t * t;

		return Vector3(x, y, z);
	}

	float Bezier::GetYValueWithX(float x)
	{
		InitDValueWithX(x);
		float t = GetTValue();
		float ret = vecPoints_[0].y_ * (1 - t) * (1 - t) * (1 - t) +
			3 * vecPoints_[1].y_ * t * (1 - t) * (1 - t) +
			3 * vecPoints_[2].y_ * t * t * (1 - t) +
			vecPoints_[3].y_ * t * t * t;
		return ret;
	}

	float Bezier::GetXValueWithY(float y)
	{
		InitDValueWithY(y);
		float t = GetTValue();
		float ret = vecPoints_[0].x_ * (1 - t) * (1 - t) * (1 - t) +
			3 * vecPoints_[1].x_ * t * (1 - t) * (1 - t) +
			3 * vecPoints_[2].x_ * t * t * (1 - t) +
			vecPoints_[3].x_ * t * t * t;
		return ret;
	}

	void Bezier::InitDValueWithX(float x)
	{
		d_ = vecPoints_[0].x_ - x;
	}

	void Bezier::InitDValueWithY(float y)
	{
		d_ = vecPoints_[0].y_ - y;
	}

	float Bezier::GetTValue()
	{
		float tVal = 0.0f;
		A_ = b_ * b_ - 3 * a_ * c_;
		B_ = b_ * c_ - 9 * a_ * d_;
		C_ = c_ * c_ - 3 * b_ * d_;
		float delta = B_ * B_ - 4 * A_ * C_;

		if (FLT_IS_ZERO(A_) && FLT_IS_ZERO(B_))
		{
			tVal = -c_ / b_;
		}
		else if (delta > 0.0f)
		{
			float y1 = A_ * b_ + 3 * a_ * ((-B_ + sqrtf(B_ * B_ - 4 * A_ * C_)) / 2.0f);
			float y2 = A_ * b_ + 3 * a_ * ((-B_ - sqrtf(B_ * B_ - 4 * A_ * C_)) / 2.0f);
			tVal = (-b_ - (pow(y1, 1.0f / 3.0f) + pow(y2, 1.0f / 3.0f))) / (3 * a_);
		}
		else if (FLT_IS_ZERO(delta))
		{
			float k = B_ / A_;
			float root = -b_ / a_ + k;
			if (root > 0.0f && root <= 1.0f)
			{
				return root;
			}

			root = -k / 2.0f;
			if (root > 0.0f && root <= 1.0f)
			{
				return root;
			}
		}
		else if (delta < 0.0f)
		{
			if (A_ > 0)
			{
				float t = (2 * A_ * b_ - 3 * a_ * B_) / (2 * sqrt(A_ * A_ * A_));
				if (t > -1.0f && t < 1.0f)
				{
					float th = acosf(t);
					float root1 = (-b_ - 2 * sqrtf(A_) * cosf(th / 3.0f)) / (3 * a_);
					float root2 = (-b_ + sqrtf(A_) * (cosf(th / 3.0f) + sqrtf(3.0f) * sinf(th / 3.0f))) / (3 * a_);
					float root3 = (-b_ + sqrtf(A_) * (cosf(th / 3.0f) - sqrtf(3.0f) * sinf(th / 3.0f))) / (3 * a_);

					if (root1 >= 0.0f && root1 <= 1.0f)
					{
						tVal = root1;
					}
					else if (root2 >= 0.0f && root2 <= 1.0f)
					{
						tVal = root2;
					}
					else if (root3 >= 0.0f && root3 <= 1.0f)
					{
						tVal = root3;
					}
				}
			}
		}
		return tVal;
	}
}