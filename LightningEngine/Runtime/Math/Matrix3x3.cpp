#include "Matrix4x4.h"
#include "Matrix3x3.h"

namespace LightningEngine
{
	Matrix3x3::Matrix3x3()
	{
		LoadIdentity();
	}

	Matrix3x3::Matrix3x3(const Matrix3x3 & mat)
	{
		memcpy(data_, mat.data_, sizeof(float) * 9);
	}

	Matrix3x3::Matrix3x3(float val)
	{
		data_[0] = val;
		data_[1] = 0.0f;
		data_[2] = 0.0f;

		data_[3] = 0.0f;
		data_[4] = val;
		data_[5] = 0.0f;

		data_[6] = 0.0f;
		data_[7] = 0.0f;
		data_[8] = val;
	}

	Matrix3x3::Matrix3x3(const Matrix4x4 & mat)
	{
		data_[0] = mat.data_[0];
		data_[1] = mat.data_[1];
		data_[2] = mat.data_[2];

		data_[3] = mat.data_[4];
		data_[4] = mat.data_[5];
		data_[5] = mat.data_[6];

		data_[6] = mat.data_[8];
		data_[7] = mat.data_[9];
		data_[8] = mat.data_[10];

	}

	float & Matrix3x3::Get(int row, int col)
	{
		return data_[row + (col * 3)];
	}

	const float & Matrix3x3::Get(int row, int col) const
	{
		return data_[row + (col * 3)];
	}

	void Matrix3x3::operator=(const Matrix3x3 & mat)
	{
		memcpy(data_, mat.data_, sizeof(float) * 9);
	}

	void Matrix3x3::operator=(const Matrix4x4 & mat)
	{
		data_[0] = mat.data_[0];
		data_[1] = mat.data_[1];
		data_[2] = mat.data_[2];

		data_[3] = mat.data_[4];
		data_[4] = mat.data_[5];
		data_[5] = mat.data_[6];

		data_[6] = mat.data_[8];
		data_[7] = mat.data_[9];
		data_[8] = mat.data_[10];
	}

	Matrix3x3 & Matrix3x3::operator*=(const Matrix3x3 & mat)
	{
		for (int i = 0; i < 3; i++)
		{
			float v[3] = { Get(i, 0), Get(i , 1), Get(i, 2) };
			Get(i, 0) = v[0] * mat.Get(0, 0) + v[1] * mat.Get(1, 0) + v[2] * mat.Get(2, 0);
			Get(i, 1) = v[0] * mat.Get(0, 1) + v[1] * mat.Get(1, 1) + v[2] * mat.Get(2, 1);
			Get(i, 2) = v[0] * mat.Get(0, 2) + v[1] * mat.Get(1, 2) + v[2] * mat.Get(2, 2);
		}

		return *this;
	}

	Matrix3x3 & Matrix3x3::operator*=(const Matrix4x4 & mat)
	{
		for (int i = 0; i < 3; i++)
		{
			float v[3] = { Get(i, 0), Get(i , 1), Get(i, 2) };
			Get(i, 0) = v[0] * mat.Get(0, 0) + v[1] * mat.Get(1, 0) + v[2] * mat.Get(2, 0);
			Get(i, 1) = v[0] * mat.Get(0, 1) + v[1] * mat.Get(1, 1) + v[2] * mat.Get(2, 1);
			Get(i, 2) = v[0] * mat.Get(0, 2) + v[1] * mat.Get(1, 2) + v[2] * mat.Get(2, 2);
		}

		return *this;
	}

	Vector3 Matrix3x3::MultiplyVector3(const Vector3 & v) const
	{
		Vector3 ret;
		ret.x_ = data_[0] * v.x_ + data_[3] * v.y_ + data_[6] * v.z_;
		ret.y_ = data_[1] * v.x_ + data_[4] * v.y_ + data_[7] * v.z_;
		ret.z_ = data_[2] * v.x_ + data_[5] * v.y_ + data_[8] * v.z_;
		return ret;
	}

	void Matrix3x3::MultiplyVector3(const Vector3 & v, Vector3 & output) const
	{
		output.x_ = data_[0] * v.x_ + data_[3] * v.y_ + data_[6] * v.z_;
		output.y_ = data_[1] * v.x_ + data_[4] * v.y_ + data_[7] * v.z_;
		output.z_ = data_[2] * v.x_ + data_[5] * v.y_ + data_[8] * v.z_;
	}

	Vector3 Matrix3x3::MultiplyPoint3(const Vector3 & v) const
	{
		return MultiplyVector3(v);
	}

	Matrix3x3 & Matrix3x3::operator*=(float v)
	{
		for (int i = 0; i < 9; i++)
		{
			data_[i] *= v;
		}

		return *this;
	}

	Matrix3x3 & Matrix3x3::operator/=(float v)
	{
		if (FLT_IS_ZERO(v))
		{
			return *this;
		}

		return (*this) *= (1.0f / v);
	}

	float Matrix3x3::GetDeteminant() const
	{
		float fCofactor0 = Get(0, 0) * Get(1, 1) * Get(2, 2);
		float fCoFactor1 = Get(0, 1) * Get(1, 2) * Get(2, 0);
		float fCoFactor2 = Get(0, 2) * Get(1, 0) * Get(2, 1);
		float fCoFactor3 = Get(0, 2) * Get(1, 1) * Get(2, 0);
		float fCoFactor4 = Get(0, 1) * Get(1, 0) * Get(2, 2);
		float fCoFactor5 = Get(0, 0) * Get(1, 2) * Get(2, 1);

		return fCofactor0 + fCoFactor1 + fCoFactor2 - 
			fCoFactor3 - fCoFactor4 - fCoFactor5;
	}

	Matrix3x3 & Matrix3x3::Transpose()
	{
		std::swap(Get(0, 1), Get(1, 0));
		std::swap(Get(0, 2), Get(2, 0));
		std::swap(Get(2, 1), Get(1, 2));
		return *this;
	}

	bool Matrix3x3::Invert()
	{
		float m11 = data_[0];
		float m12 = data_[1];
		float m13 = data_[2];
		float m21 = data_[3];
		float m22 = data_[4];
		float m23 = data_[5];
		float m31 = data_[6];
		float m32 = data_[7];
		float m33 = data_[8];

		data_[0] = m22 * m33 - m32 * m23;
		data_[1] = m32 * m13 - m12 * m33;
		data_[2] = m12 * m23 - m22 * m13;
		data_[3] = m31 * m23 - m21 * m33;
		data_[4] = m11 * m33 - m31 * m13;
		data_[5] = m21 * m13 - m11 * m23;
		data_[6] = m21 * m32 - m31 * m22;
		data_[7] = m31 * m12 - m11 * m32;
		data_[8] = m11 * m22 - m21 * m12;

		return true;
	}
	void Matrix3x3::InvertTranspose()
	{
		Invert();
		Transpose();
	}

	void Matrix3x3::LoadIdentity()
	{
		data_[0] = 1.0f;
		data_[1] = 0.0f;
		data_[2] = 0.0f;
		data_[3] = 0.0f;
		data_[4] = 1.0f;
		data_[5] = 0.0f;
		data_[6] = 0.0f;
		data_[7] = 0.0f;
		data_[8] = 1.0f;
	}

	bool Matrix3x3::IsIdentity(float threshold)
	{
		if (EqualApproximately(Get(0, 0), 1.0f, threshold) &&
			EqualApproximately(Get(0, 1), 0.0f, threshold) &&
			EqualApproximately(Get(0, 2), 0.0f, threshold) &&
			EqualApproximately(Get(1, 0), 0.0f, threshold) &&
			EqualApproximately(Get(1, 1), 1.0f, threshold) &&
			EqualApproximately(Get(1, 2), 0.0f, threshold) &&
			EqualApproximately(Get(2, 0), 0.0f, threshold) &&
			EqualApproximately(Get(2, 1), 0.0f, threshold) &&
			EqualApproximately(Get(2, 2), 1.0f, threshold))
		{
			return true;
		}
			
		return false;
	}

	Matrix3x3 operator*(const Matrix3x3 & lhs, const Matrix3x3 & rhs)
	{
		Matrix3x3 tmp(lhs);
		tmp *= rhs;
		return tmp;
	}

	void RotationMatrix(const Vector3 & from, const Vector3 & to, float * rotation_mat)
	{

	}

	bool MatrixToEuler(const Matrix3x3 & mat, Vector3 & v)
	{
		if (mat.Get(1, 2) < 0.999f)
		{
			if (mat.Get(1, 2) > -0.999f)
			{
				v.x_ = asin(-mat.Get(1, 2));
				v.y_ = atan2(mat.Get(0, 2), mat.Get(2, 2));
				v.z_ = atan2(mat.Get(1, 0), mat.Get(1, 1));
				MakePositive(v);

				return true;
			}

			v.x_ = ENGINE_PI * 0.5f;
			v.y_ = atan2(mat.Get(0, 1), mat.Get(0, 0));
			v.z_ = 0.0f;
			MakePositive(v);

			return false;
		}

		v.x_ = -ENGINE_PI * 0.5f;
		v.y_ = atan2(-mat.Get(0, 1), mat.Get(0, 0));
		v.z_ = 0.0f;
		MakePositive(v);

		return false;
	}

	void EulerToMatrix(const Vector3 & v, Matrix3x3 & mat)
	{
		float cx = cos(v.x_);
		float sx = sin(v.x_);
		float cy = cos(v.y_);
		float sy = sin(v.y_);
		float cz = cos(v.z_);
		float sz = sin(v.z_);

		mat.Get(0, 0) = cy * cz + sx * sy * sz;
		mat.Get(0, 1) = cz * sx * sy - cy * sz;
		mat.Get(0, 2) = cx * sy;

		mat.Get(1, 0) = cx * sz;
		mat.Get(1, 1) = cx * cz;
		mat.Get(1, 2) = -sx;

		mat.Get(2, 0) = -cz * sy + cy * sx * sz;
		mat.Get(2, 1) = cy * cz * sx + sy * sz;
		mat.Get(2, 2) = cx * cy;
	}

	void FromToRotation(const float * from, const float * to, float mat[3][3])
	{
		float v[3];
		float e, h;
		VEC3_CROSS(v, from, to);
		e = VEC3_DOT(from, to);

		if (e > 1.0 - ENGINE_EPSILON)
		{
			mat[0][0] = 1.0f;
			mat[0][1] = 0.0f;
			mat[0][2] = 0.0f;

			mat[1][0] = 0.0f;
			mat[1][1] = 1.0f;
			mat[1][2] = 0.0f;

			mat[2][0] = 0.0f;
			mat[2][1] = 0.0f;
			mat[2][2] = 1.0f;
		}
		else if (e < -1.0 + ENGINE_EPSILON)
		{
			float up[3], left[3];
			float invLen;
			float fxx, fyy, fzz, fxy, fxz, fyz;
			float uxx, uyy, uzz, uxy, uxz, uyz;
			float lxx, lyy, lzz, lxy, lxz, lyz;
			left[0] = 0.0f;
			left[1] = from[2];
			left[2] = -from[1];

			if (VEC3_DOT(left, left) < ENGINE_EPSILON)
			{
				left[0] = -from[2];
				left[1] = 0.0f;
				left[2] = from[0];
			}

			invLen = 1.0f / sqrt(VEC3_DOT(left, left));
			left[0] *= invLen;
			left[1] *= invLen;
			left[2] *= invLen;

			VEC3_CROSS(up, left, from);
			fxx = -from[0] * from[0];
			fyy = -from[1] * from[1];
			fzz = -from[2] * from[2];
			fxy = -from[0] * from[1];
			fxz = -from[0] * from[2];
			fyz = -from[1] * from[2];

			uxx = up[0] * up[0];
			uyy = up[1] * up[1];
			uzz = up[2] * up[2];
			uxy = up[0] * up[1];
			uxz = up[0] * up[2];
			uyz = up[1] * up[2];

			lxx = -left[0] * left[0];
			lyy = -left[1] * left[1];
			lzz = -left[2] * left[2];
			lxy = -left[0] * left[1];
			lxz = -left[0] * left[2];
			lyz = -left[1] * left[2];

			mat[0][0] = fxx + uxx + lxx;
			mat[0][1] = fxy + uxy + lxy;
			mat[0][2] = fxz + uyz + lxz;

			mat[1][0] = mat[0][1];
			mat[1][1] = fyy + uyy + lyy;
			mat[1][2] = fyz + uyz + lyz;
			
			mat[2][0] = mat[0][2];
			mat[2][1] = mat[1][2];
			mat[2][2] = fzz + uzz + lzz;

		}
		else
		{
			float hvx, hvz, hvxy, hvxz, hvyz;
			h = (1.0f - e) / VEC3_DOT(v, v);
			hvx = h * v[0];
			hvz = h * v[2];
			hvxy = hvx * v[1];
			hvxz = hvx * v[2];
			hvyz = hvz * v[1];

			mat[0][0] = e + hvx * v[0];
			mat[0][1] = hvxy - v[2];
			mat[0][2] = hvxz + v[1];
			
			mat[1][0] = hvxy + v[2];
			mat[1][1] = e + h * v[1] * v[1];
			mat[1][2] = hvyz - v[0];
			
			mat[2][0] = hvxz - v[1];
			mat[2][1] = hvyz + v[0];
			mat[2][2] = e + hvz * v[2];
		}
	}

	void MakePositive(Vector3 & euler)
	{
		const float negativeFlip = -0.0001f;
		const float positiveFlip = (ENGINE_PI * 2.0f) - 0.0001f;

		if (euler.x_ < negativeFlip)
		{
			euler.x_ += 2.0 * ENGINE_PI;
		}
		else if (euler.x_ > positiveFlip)
		{
			euler.x_ -= 2.0 * ENGINE_PI;
		}

		if (euler.y_ < negativeFlip)
		{
			euler.y_ += 2.0 * ENGINE_PI;
		}
		else if (euler.y_ > positiveFlip)
		{
			euler.y_ -= 2.0 * ENGINE_PI;
		}

		if (euler.z_ < negativeFlip)
		{
			euler.z_ += 2.0 * ENGINE_PI;
		}
		else if (euler.z_ > positiveFlip)
		{
			euler.z_ -= 2.0 * ENGINE_PI;
		}
	}
}