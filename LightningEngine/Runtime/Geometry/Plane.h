#pragma once

#include "Math/Vector3.h"

namespace LightningEngine
{
	/*
		平面方程：点--法线形式
		法线normal点乘 平面上的向量vec = 0；
		假设normal的分量分别为a, b, c; vec的分量分别为 x - x0, y - y0, z - z0，
		其中x0,y0,z0为已知常量; x, y, z未未知数
		点乘展开公式为: a*(x - x0) + b*(y - y0) + c*(z - z0) = 0;
		化简后可得: a*x + b*y + c*z + (-a*x0 - b*y0 - c*z0) = 0
		可设(-a*x0 - b*y0 - c*z0) = d;
		综合可得平面方程为: a*x + b*y + c*z + d = 0;
	*/
	class Plane
	{
	public:
		Plane() {}
		Plane(float a, float b, float c, float d);
		Plane& operator*=(float fScale);
		bool operator==(const Plane& plane);
		bool operator!=(const Plane& plane);
		void SetInvalid();
		void SetABCD(const float a, const float b, const float c, const float d);
		void Normalize();
		void SetNormalAndPosition(const Vector3& normal, const Vector3& point);
		float GetDistanceToPoint(const Vector3& point)const;
		bool GetSide(const Vector3& point)const;

		const float& A()const { return normal_.x_; }
		const float& B()const { return normal_.y_; }
		const float& C()const { return normal_.z_; }
		const float& D()const { return distance_; }
		const Vector3& GetNormal()const { return normal_; }

	public:
		Vector3 normal_;
		float distance_;

	};
}