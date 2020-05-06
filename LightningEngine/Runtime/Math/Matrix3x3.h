#pragma once

#include "Vector3.h"

namespace LightningEngine
{
	class Matrix4x4;
	class Matrix3x3
	{
	public:
		Matrix3x3();
		Matrix3x3(const Matrix3x3& mat);
		Matrix3x3(float val);
		Matrix3x3(const Matrix4x4& mat);
		float& Get(int row, int col);
		const float& Get(int row, int col)const;
		void operator=(const Matrix4x4& mat);
		void operator=(const Matrix3x3& mat);
		Matrix3x3& operator*=(const Matrix3x3& mat);
		Matrix3x3& operator*=(const Matrix4x4& mat);
		friend Matrix3x3 operator*(const Matrix3x3& lhs, const Matrix3x3& rhs);
		Vector3 MultiplyVector3(const Vector3& v)const;
		void MultiplyVector3(const Vector3& v, Vector3& output)const;
		Vector3 MultiplyPoint3(const Vector3& v)const;
		Matrix3x3& operator*=(float v);
		Matrix3x3& operator/=(float v);
		float GetDeteminant()const;
		Matrix3x3& Transpose();
		bool Invert();
		void InvertTranspose();
		void LoadIdentity();
		bool IsIdentity(float threshold = 0.00001f);

	public:
		float data_[9];
	};

	void RotationMatrix(const Vector3& from, const Vector3& to, float* rotation_mat);
	bool MatrixToEuler(const Matrix3x3& mat, Vector3& v);
	void EulerToMatrix(const Vector3& v, Matrix3x3& mat);
	void FromToRotation(const float* from, const float* to, float mat[3][3]);
	void MakePositive(Vector3& euler);
}