#pragma once

#include "Math/Vector3.h"

namespace LightningEngine
{
	/*
		ƽ�淽�̣���--������ʽ
		����normal��� ƽ���ϵ�����vec = 0��
		����normal�ķ����ֱ�Ϊa, b, c; vec�ķ����ֱ�Ϊ x - x0, y - y0, z - z0��
		����x0,y0,z0Ϊ��֪����; x, y, zδδ֪��
		���չ����ʽΪ: a*(x - x0) + b*(y - y0) + c*(z - z0) = 0;
		�����ɵ�: a*x + b*y + c*z + (-a*x0 - b*y0 - c*z0) = 0
		����(-a*x0 - b*y0 - c*z0) = d;
		�ۺϿɵ�ƽ�淽��Ϊ: a*x + b*y + c*z + d = 0;
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