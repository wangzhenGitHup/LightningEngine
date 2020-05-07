#pragma once

#include "Float.h"
#include "Vector3.h"
#include "Vector4.h"

#define ENGINE_MAT4_DATA(mat, row, col) (mat)[(col) * 4 + (row)]

namespace LightningEngine
{
	class Matrix3x3;
	class Matrix4x4
	{
	public:
		Matrix4x4();
		Matrix4x4(const Matrix4x4& mat);
		Matrix4x4(const Matrix3x3& mat);
		Vector3 operator*(Vector3& v);
		Vector3 operator*(const Vector4& v)const;
		float& Get(int row, int col);
		const float& Get(int row, int col)const;
		Matrix4x4& operator=(const Matrix3x3& mat);
		void operator*=(const Matrix4x4& mat);
		Matrix4x4& operator=(const Matrix4x4& mat);
		double GetDeterminant()const;
		Vector3 MultiplyVector3(const Vector3& v)const;
		Vector3 MultiplyPoint3(const Vector3& v)const;
		void MultiplyPoint3(const Vector3& v, Vector3& output)const;
		bool PerspectiveMultiplyPoint3(const Vector3& v, Vector3& output)const;
		Vector3 InverseMultiplyPoint3Affine(const Vector3& v)const;
		Vector3 InverMultiplyVector3Affine(const Vector3& v)const;

		void LookAt(Vector3& eye, Vector3& lookAt, Vector3& up);
		void SetPerspective(float fovy, float aspect, float zNear, float zFar);
		void SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
		void LoadIdentity();
		Matrix4x4& LocalTranslate(const Vector3& trans);
		Matrix4x4& LocalTranslate(float x, float y, float z);
		void Transpose();
		void SetRotationPart(float* data);
		void SetTranslatePart(float x, float y, float z);
		Matrix4x4& Scale(float x, float y, float z);
		Matrix4x4& SetScale(float x, float y, float z);
		Matrix4x4& Rotate(Vector3& from, Vector3& to);
		void Rotate(Vector4& v);
		void Dump(const char* hint = "matrix");


	public:
		float data_[16];
	};

	void Matrix4x4MultiplyMatrix4x4(const float* left, const float* right, float* ret);
	bool InvertMatrix4x4(const float* mat, float* out);
}