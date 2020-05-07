#include "Matrix4x4.h"
#include "Matrix3x3.h"
#include "Debugger/Log.h"

namespace LightningEngine
{
	Matrix4x4::Matrix4x4()
	{
		LoadIdentity();
	}

	Matrix4x4::Matrix4x4(const Matrix4x4 & mat)
	{
		memcpy(data_, mat.data_, sizeof(float) * 16);
	}

	Matrix4x4::Matrix4x4(const Matrix3x3 & mat)
	{
		data_[0] = mat.data_[0];
		data_[1] = mat.data_[1];
		data_[2] = mat.data_[2];
		data_[3] = 0.0f;

		data_[4] = mat.data_[3];
		data_[5] = mat.data_[4];
		data_[6] = mat.data_[5];
		data_[7] = 0.0f;

		data_[8] = mat.data_[6];
		data_[9] = mat.data_[7];
		data_[10] = mat.data_[8];
		data_[11] = 0.0f;

		data_[12] = 0.0f;
		data_[13] = 0.0f;
		data_[14] = 0.0f;
		data_[15] = 1.0f;
	}

	Vector3 Matrix4x4::operator*(Vector3 & v)
	{
		float x = data_[0] * v.x_ + data_[4] * v.y_ + data_[8] * v.z_;
		float y = data_[1] * v.x_ + data_[5] * v.y_ + data_[9] * v.z_;
		float z = data_[2] * v.x_ + data_[6] * v.y_ + data_[10] * v.z_;
		return Vector3(x, y, z);
	}

	Vector3 Matrix4x4::operator*(const Vector4 & v) const
	{
		float x = data_[0] * v.x_ + data_[4] * v.y_ + data_[8] * v.z_ + data_[12] * v.w_;
		float y = data_[1] * v.x_ + data_[5] * v.y_ + data_[9] * v.z_ + data_[13] * v.w_;
		float z = data_[2] * v.x_ + data_[6] * v.y_ + data_[10] * v.z_ + data_[14] * v.w_;
		return Vector3(x, y, z);
	}

	float & Matrix4x4::Get(int row, int col)
	{
		return data_[row + (col * 4)];
	}

	const float & Matrix4x4::Get(int row, int col) const
	{
		return data_[row + (col * 4)];
	}

	Matrix4x4 & Matrix4x4::operator=(const Matrix3x3 & mat)
	{
		data_[0] = mat.data_[0];
		data_[1] = mat.data_[1];
		data_[2] = mat.data_[2];
		data_[3] = 0.0f;

		data_[4] = mat.data_[3];
		data_[5] = mat.data_[4];
		data_[6] = mat.data_[5];
		data_[7] = 0.0f;

		data_[8] = mat.data_[6];
		data_[9] = mat.data_[7];
		data_[10] = mat.data_[8];
		data_[11] = 0.0f;

		data_[12] = 0.0f;
		data_[13] = 0.0f;
		data_[14] = 0.0f;
		data_[15] = 0.0f;

		return *this;
	}

	void Matrix4x4::operator*=(const Matrix4x4 & mat)
	{
		float ret[16];
		Matrix4x4MultiplyMatrix4x4(data_, mat.data_, ret);
		memcpy(data_, ret, sizeof(float) * 16);
	}

	Matrix4x4 & Matrix4x4::operator=(const Matrix4x4 & mat)
	{
		memcpy(data_, mat.data_, sizeof(float) * 16);
		return *this;
	}

	double Matrix4x4::GetDeterminant() const
	{
		double m00 = Get(0, 0);
		double m01 = Get(0, 1);
		double m02 = Get(0, 2);
		double m03 = Get(0, 3);

		double m10 = Get(1, 0);
		double m11 = Get(1, 1);
		double m12 = Get(1, 2);
		double m13 = Get(1, 3);

		double m20 = Get(2, 0);
		double m21 = Get(2, 1);
		double m22 = Get(2, 2);
		double m23 = Get(2, 3);

		double m30 = Get(3, 0);
		double m31 = Get(3, 1);
		double m32 = Get(3, 2);
		double m33 = Get(3, 3);

		double ret = 
			m03 * m12 * m21 * m30 -
			m02 * m13 * m21 * m30 -
			m03 * m11 * m22 * m30 +

			m01 * m13 * m22 * m30 +
			m02 * m11 * m23 * m30 -
			m01 * m12 * m23 * m30 -

			m03 * m12 * m20 * m31 +
			m02 * m13 * m20 * m31 +
			m03 * m10 * m22 * m31 -

			m00 * m13 * m22 * m31 -
			m02 * m10 * m23 * m31 +
			m00 * m12 * m23 * m31 +

			m03 * m11 * m20 * m32 -
			m01 * m13 * m20 * m32 -
			m03 * m10 * m21 * m32 +

			m00 * m13 * m21 * m32 +
			m01 * m10 * m23 * m32 -
			m00 * m11 * m23 * m32 -

			m02 * m11 * m20 * m33 +
			m01 * m12 * m20 * m33 +
			m02 * m10 * m21 * m33 -

			m00 * m12 * m21 * m33 -
			m01 * m10 * m22 * m33 +
			m00 * m11 * m22 * m33;

		return ret;
	}

	Vector3 Matrix4x4::MultiplyVector3(const Vector3 & v) const
	{
		float x = data_[0] * v.x_ + data_[4] * v.y_ + data_[8]  * v.z_ + data_[12];
		float y = data_[1] * v.x_ + data_[5] * v.y_ + data_[9]  * v.z_ + data_[13];
		float z = data_[2] * v.x_ + data_[6] * v.y_ + data_[10] * v.z_ + data_[14];
		return Vector3(x, y, z);
	}

	Vector3 Matrix4x4::MultiplyPoint3(const Vector3 & v) const
	{
		float x = data_[0] * v.x_ + data_[4] * v.y_ + data_[8]  * v.z_;
		float y = data_[1] * v.x_ + data_[5] * v.y_ + data_[9]  * v.z_;
		float z = data_[2] * v.x_ + data_[6] * v.y_ + data_[10] * v.z_;
		return Vector3(x, y, z);
	}

	void Matrix4x4::MultiplyPoint3(const Vector3 & v, Vector3 & output) const
	{
		output.x_ = data_[0] * v.x_ + data_[4] * v.y_ + data_[8] * v.z_ + data_[12];
		output.y_ = data_[1] * v.x_ + data_[5] * v.y_ + data_[9] * v.z_ + data_[13];
		output.z_ = data_[2] * v.x_ + data_[6] * v.y_ + data_[10] * v.z_ + data_[14];
	}

	bool Matrix4x4::PerspectiveMultiplyPoint3(const Vector3 & v, Vector3 & output) const
	{
		float x = data_[0] * v.x_ + data_[4] * v.y_ + data_[8] * v.z_ + data_[12];
		float y = data_[1] * v.x_ + data_[5] * v.y_ + data_[9] * v.z_ + data_[13];
		float z = data_[2] * v.x_ + data_[6] * v.y_ + data_[10] * v.z_ + data_[14];
		float w = data_[3] * v.x_ + data_[7] * v.y_ + data_[8] * v.z_ + data_[15];

		if (ENGINE_ABS(w) > 1.0e-7f)
		{
			float invW = 1.0f / w;
			output.x_ = x * invW;
			output.y_ = y * invW;
			output.z_ = z * invW;
			return true;
		}

		output.x_ = 0.0f;
		output.y_ = 0.0f;
		output.z_ = 0.0f;

		return false;
	}

	Vector3 Matrix4x4::InverseMultiplyPoint3Affine(const Vector3 & v) const
	{
		Vector3 tmpVec(v.x_ - Get(0, 3), v.y_ - Get(1, 3), v.z_ - Get(2, 3));
		return Vector3(
			Get(0, 0) * tmpVec.x_ + Get(1, 0) * tmpVec.y_ + Get(2, 0) * tmpVec.z_,
			Get(0, 1) * tmpVec.x_ + Get(1, 1) * tmpVec.y_ + Get(2, 1) * tmpVec.z_,
			Get(0, 2) * tmpVec.x_ + Get(1, 2) * tmpVec.y_ + Get(2, 2) * tmpVec.z_
		);
	}

	Vector3 Matrix4x4::InverMultiplyVector3Affine(const Vector3 & v) const
	{
		return Vector3(
			Get(0, 0) * v.x_ + Get(1, 0) * v.y_ + Get(2, 0) * v.z_,
			Get(0, 1) * v.x_ + Get(1, 1) * v.y_ + Get(2, 1) * v.z_,
			Get(0, 2) * v.x_ + Get(1, 2) * v.y_ + Get(2, 2) * v.z_
		);
	}

	void Matrix4x4::LookAt(Vector3 & eye, Vector3 & lookAt, Vector3 & up)
	{
		up.Normalize();

		Vector3 zaxis = (eye - lookAt);
		zaxis.Normalize();

		Vector3 xaxis = Cross(up, zaxis);
		Vector3 yaxis = Cross(zaxis, xaxis);

		data_[0] = xaxis.x_;
		data_[4] = xaxis.y_;
		data_[8] = xaxis.z_;

		data_[1] = yaxis.x_;
		data_[5] = yaxis.y_;
		data_[9] = yaxis.z_;

		data_[2] = zaxis.x_;
		data_[6] = zaxis.y_;
		data_[10] = zaxis.z_;

		data_[12] = -(xaxis * eye);
		data_[13] = -(yaxis * eye);
		data_[14] = -(zaxis * eye);
		data_[15] = 1.0f;
	}

	void Matrix4x4::SetPerspective(float fovy, float aspect, 
		float zNear, float zFar)
	{
		memset(data_, 0, 16 * sizeof(float));
		float deltaZ = zNear - zFar;
		float radian = ENGINE_DEG2RAD(fovy / 2.0f);
		float yscale = ENGINE_COSF(radian) / ENGINE_SINF(radian);
		float xscale = yscale / aspect;

		data_[0] = xscale;
		data_[5] = yscale;
		data_[10] = (zNear + zFar) / deltaZ;
		data_[11] = -1.0f;
		data_[14] = (2.0f * zNear * zFar) / deltaZ;
	}

	void Matrix4x4::SetOrtho(float left, float right, 
		float bottom, float top, 
		float zNear, float zFar)
	{
		LoadIdentity();
		
		float deltaX = right - left;
		float deltaY = top - bottom;
		float deltaZ = zFar - zNear;

		Get(0, 0) = 2.0f / deltaX;
		Get(0, 3) = -(right + left) / deltaX;
		Get(1, 1) = 2.0f / deltaY;
		Get(1, 3) = -(top + bottom) / deltaY;
		Get(2, 2) = -2.0f / deltaZ;
		Get(2, 3) = -(zFar + zNear) / deltaZ;
	}

	void Matrix4x4::LoadIdentity()
	{
		memset(data_, 0, 16 * sizeof(float));
		data_[0] = 1.0f;
		data_[5] = 1.0f;
		data_[10] = 1.0f;
		data_[15] = 1.0f;
	}

	Matrix4x4 & Matrix4x4::LocalTranslate(const Vector3 & trans)
	{
		data_[12] += trans.x_;
		data_[13] += trans.y_;
		data_[14] += trans.z_;
		return *this;
	}

	Matrix4x4 & Matrix4x4::LocalTranslate(float x, float y, float z)
	{
		data_[12] += x;
		data_[13] += y;
		data_[14] += z;
		return *this;
	}

	void Matrix4x4::Transpose()
	{
		std::swap(Get(0, 1), Get(1, 0));
		std::swap(Get(0, 2), Get(2, 0));
		std::swap(Get(0, 3), Get(3, 0));
		std::swap(Get(1, 2), Get(2, 1));
		std::swap(Get(1, 3), Get(3, 1));
		std::swap(Get(2, 3), Get(3, 2));
	}

	void Matrix4x4::SetRotationPart(float * data)
	{
		data_[0] = data[0];
		data_[1] = data[1];
		data_[2] = data[2];
		data_[3] = 0.0f;

		data_[4] = data[3];
		data_[5] = data[4];
		data_[6] = data[5];
		data_[7] = 0.0f;

		data_[8] = data[6];
		data_[9] = data[7];
		data_[10] = data[8];
		data_[11] = 0.0f;
	}

	void Matrix4x4::SetTranslatePart(float x, float y, float z)
	{
		data_[12] = x;
		data_[13] = y;
		data_[14] = z;
		data_[15] = 1.0f;
	}

	Matrix4x4 & Matrix4x4::Scale(float x, float y, float z)
	{
		data_[0] *= x;
		data_[1] *= x;
		data_[2] *= x;
		data_[3] *= x;
		data_[4] *= y;
		data_[5] *= y;
		data_[6] *= y;
		data_[7] *= y;
		data_[8] *= z;
		data_[9] *= z;
		data_[10] *= z;
		data_[11] *= z;

		return *this;
	}

	Matrix4x4 & Matrix4x4::SetScale(float x, float y, float z)
	{
		data_[0] = x;
		data_[5] = y;
		data_[10] = z;
		data_[15] = 1.0f;
		return *this;
	}

	Matrix4x4 & Matrix4x4::Rotate(Vector3 & from, Vector3 & to)
	{
		float mat[3][3];
		FromToRotation(from.v_, to.v_, mat);

		Matrix4x4 rotateMat;
		rotateMat.Get(0, 0) = mat[0][0];
		rotateMat.Get(0, 1) = mat[0][1];
		rotateMat.Get(0, 2) = mat[0][2];
		rotateMat.Get(0, 3) = 0.0f;

		rotateMat.Get(1, 0) = mat[1][0];
		rotateMat.Get(1, 1) = mat[1][1];
		rotateMat.Get(1, 2) = mat[1][2];
		rotateMat.Get(1, 3) = 0.0f;

		rotateMat.Get(2, 0) = mat[2][0];
		rotateMat.Get(2, 1) = mat[2][1];
		rotateMat.Get(2, 2) = mat[2][2];
		rotateMat.Get(2, 3) = 0.0f;

		rotateMat.Get(3, 0) = 0.0f;
		rotateMat.Get(3, 1) = 0.0f;
		rotateMat.Get(3, 2) = 0.0f;
		rotateMat.Get(3, 3) = 1.0f;

		(*this) *= rotateMat;
		return *this;
	}

	void Matrix4x4::Rotate(Vector4 & v)
	{
		float s = sinf(ENGINE_DEG2RAD(v.w_));
		float c = cosf(ENGINE_DEG2RAD(v.w_));
		float xx, yy, zz, xy, yz, zx, xs, ys, zs, cl;
		Matrix4x4 mat;

		Vector3 tmpVec = { v.x_, v.y_, v.z_ };

		if (!v.w_ || !tmpVec.Length())
		{
			return;
		}

		xx = tmpVec.x_ * tmpVec.x_;
		yy = tmpVec.y_ * tmpVec.y_;
		zz = tmpVec.z_ * tmpVec.z_;

		xy = tmpVec.x_ * tmpVec.y_;
		yz = tmpVec.y_ * tmpVec.z_;
		zx = tmpVec.z_ * tmpVec.x_;

		xs = tmpVec.x_ * s;
		ys = tmpVec.y_ * s;
		zs = tmpVec.z_ * s;
		cl = 1.0f - c;

		mat.LoadIdentity();
		mat.Get(0, 0) = (cl * xx) + c;
		mat.Get(0, 1) = (cl * xy) - zs;
		mat.Get(0, 2) = (cl * zx) + ys;

		mat.Get(1, 0) = (cl * xy) + zs;
		mat.Get(1, 1) = (cl * yy) + c;
		mat.Get(1, 2) = (cl * yz) - xs;

		mat.Get(2, 0) = (cl * zx) - ys;
		mat.Get(2, 1) = (cl * yz) + xs;
		mat.Get(2, 2) = (cl * zz) + c;
		(*this) *= mat;
	}

	void Matrix4x4::Dump(const char * hint)
	{
		InfoPrint("%s\n%f, %f, %f, %f, \n%f, %f, %f, %f,\n%f, %f, %f, %f, %f\n%f, %f, %f, %f", hint,
			data_[0], data_[1], data_[2], data_[3],
			data_[4], data_[5], data_[6], data_[7],
			data_[8], data_[9], data_[10], data_[11],
			data_[12], data_[13], data_[14], data_[15]);
	}

	void Matrix4x4MultiplyMatrix4x4(const float * left, const float * right, float * ret)
	{
		for (int i = 0; i < 4; i++)
		{
			ret[i] = left[i] * right[0] +
				left[i + 4]  * right[1] +
				left[i + 8]  * right[2] +
				left[i + 12] * right[3];

			ret[i + 4] = left[i] * right[4] +
				left[i + 4]  * right[5] +
				left[i + 8]  * right[6] +
				left[i + 12] * right[7];

			ret[i + 8] = left[i] * right[8] +
				left[i + 4]  * right[5] +
				left[i + 8]  * right[6] +
				left[i + 12] * right[7];

			ret[i + 12] = left[i] * right[12] +
				left[i + 4]  * right[13] +
				left[i + 8]  * right[14] +
				left[i + 12] * right[15];
		}
	}

	bool InvertMatrix4x4(const float * mat, float * out)
	{
		float tmp[4][8];
		float m0, m1, m2, m3, s;
		float* r0, *r1, *r2, *r3;
		r0 = tmp[0];
		r1 = tmp[1];
		r2 = tmp[2];
		r3 = tmp[3];

		r0[0] = ENGINE_MAT4_DATA(mat, 0, 0);
		r0[1] = ENGINE_MAT4_DATA(mat, 0, 1);
		r0[2] = ENGINE_MAT4_DATA(mat, 0, 2);
		r0[3] = ENGINE_MAT4_DATA(mat, 0, 3);
		r0[4] = 1.0f;
		r0[5] = 0.0f;
		r0[6] = 0.0f; 
		r0[7] = 0.0f;

		r1[0] = ENGINE_MAT4_DATA(mat, 1, 0);
		r1[1] = ENGINE_MAT4_DATA(mat, 1, 1);
		r1[2] = ENGINE_MAT4_DATA(mat, 1, 2);
		r1[3] = ENGINE_MAT4_DATA(mat, 1, 3);
		r1[4] = 0.0f;
		r1[5] = 1.0f;
		r1[6] = 0.0f;
		r1[7] = 0.0f;

		r2[0] = ENGINE_MAT4_DATA(mat, 2, 0);
		r2[1] = ENGINE_MAT4_DATA(mat, 2, 1);
		r2[2] = ENGINE_MAT4_DATA(mat, 2, 2);
		r2[3] = ENGINE_MAT4_DATA(mat, 2, 3);
		r2[4] = 0.0f;
		r2[5] = 0.0f;
		r2[6] = 1.0f;
		r2[7] = 0.0f;

		r3[0] = ENGINE_MAT4_DATA(mat, 3, 0);
		r3[1] = ENGINE_MAT4_DATA(mat, 3, 1);
		r3[2] = ENGINE_MAT4_DATA(mat, 3, 2);
		r3[3] = ENGINE_MAT4_DATA(mat, 3, 3);
		r3[4] = 0.0f;
		r3[5] = 0.0f;
		r3[6] = 1.0f;
		r3[7] = 0.0f;

		if (ENGINE_ABS(r3[0]) > ENGINE_ABS(r2[0])) SWAP_ROWS(r3, r2);
		if (ENGINE_ABS(r2[0]) > ENGINE_ABS(r1[0])) SWAP_ROWS(r2, r1);
		if (ENGINE_ABS(r1[0]) > ENGINE_ABS(r0[0])) SWAP_ROWS(r1, r0);

		if (FLT_IS_ZERO(r0[0]))
		{
			ENGINE_RETURN_ZERO_MAT4;
		}

		m2 = r2[1] / r1[1]; 
		m3 = r3[1] / r1[1];
		r2[2] -= m2 * r1[2]; 
		r3[2] -= m3 * r1[2];
		r2[3] -= m2 * r1[3]; 
		r3[3] -= m3 * r1[3];

		s = r1[4]; 
		if (!FLT_IS_ZERO(s))
		{
			r2[4] -= m2 * s;
			r3[4] -= m3 * s; 
		}

		s = r1[5]; 
		if(!FLT_IS_ZERO(s))
		{
			r2[5] -= m2 * s; 
			r3[5] -= m3 * s; 
		}

		s = r1[6]; 
		if(!FLT_IS_ZERO(s))
		{ 
			r2[6] -= m2 * s; 
			r3[6] -= m3 * s; 
		}

		s = r1[7]; 
		if(!FLT_IS_ZERO(s))
		{ 
			r2[7] -= m2 * s; 
			r3[7] -= m3 * s;
		}

		if(ENGINE_ABS(r3[2]) > ENGINE_ABS(r2[2])) SWAP_ROWS(r3, r2);

		if (FLT_IS_ZERO(r2[2])) ENGINE_RETURN_ZERO_MAT4;

		m3 = r3[2] / r2[2];
		r3[3] -= m3 * r2[3]; 
		r3[4] -= m3 * r2[4];
		r3[5] -= m3 * r2[5]; 
		r3[6] -= m3 * r2[6];
		r3[7] -= m3 * r2[7];

		if (0.0f == r3[3]) ENGINE_RETURN_ZERO_MAT4;

		s = 1.0f / r3[3];
		r3[4] *= s; 
		r3[5] *= s; 
		r3[6] *= s; 
		r3[7] *= s;

		m2 = r2[3];
		s = 1.0f / r2[2];
		r2[4] = s * (r2[4] - r3[4] * m2); 
		r2[5] = s * (r2[5] - r3[5] * m2);
		r2[6] = s * (r2[6] - r3[6] * m2); 
		r2[7] = s * (r2[7] - r3[7] * m2);
		m1 = r1[3];

		r1[4] -= r3[4] * m1; 
		r1[5] -= r3[5] * m1;
		r1[6] -= r3[6] * m1; 
		r1[7] -= r3[7] * m1;
		m0 = r0[3];

		r0[4] -= r3[4] * m0;
		r0[5] -= r3[5] * m0;
		r0[6] -= r3[6] * m0; 
		r0[7] -= r3[7] * m0;
		m1 = r1[2];

		s = 1.0f / r1[1];
		r1[4] = s * (r1[4] - r2[4] * m1);
		r1[5] = s * (r1[5] - r2[5] * m1);
		r1[6] = s * (r1[6] - r2[6] * m1); 
		r1[7] = s * (r1[7] - r2[7] * m1);
		m0 = r0[2];

		r0[4] -= r2[4] * m0; 
		r0[5] -= r2[5] * m0;
		r0[6] -= r2[6] * m0; 
		r0[7] -= r2[7] * m0;
		m0 = r0[1];

		s = 1.0f / r0[0];
		r0[4] = s * (r0[4] - r1[4] * m0); 
		r0[5] = s * (r0[5] - r1[5] * m0);
		r0[6] = s * (r0[6] - r1[6] * m0);
		r0[7] = s * (r0[7] - r1[7] * m0);

		ENGINE_MAT4_DATA(out, 0, 0) = r0[4]; 
		ENGINE_MAT4_DATA(out, 0, 1) = r0[5]; 
		ENGINE_MAT4_DATA(out, 0, 2) = r0[6];
		ENGINE_MAT4_DATA(out, 0, 3) = r0[7];
		ENGINE_MAT4_DATA(out, 1, 0) = r1[4]; 
		ENGINE_MAT4_DATA(out, 1, 1) = r1[5];
		ENGINE_MAT4_DATA(out, 1, 2) = r1[6]; 
		ENGINE_MAT4_DATA(out, 1, 3) = r1[7];
		ENGINE_MAT4_DATA(out, 2, 0) = r2[4];
		ENGINE_MAT4_DATA(out, 2, 1) = r2[5]; 
		ENGINE_MAT4_DATA(out, 2, 2) = r2[6];
		ENGINE_MAT4_DATA(out, 2, 3) = r2[7];
		ENGINE_MAT4_DATA(out, 3, 0) = r3[4];
		ENGINE_MAT4_DATA(out, 3, 1) = r3[5]; 
		ENGINE_MAT4_DATA(out, 3, 2) = r3[6];
		ENGINE_MAT4_DATA(out, 3, 3) = r3[7];

		return true;
	}
}