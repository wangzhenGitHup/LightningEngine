#pragma once

#include "Base/Object.h"
#include "Math/Vector3.h"
#include "Point3.h"

namespace LightningEngine
{
	class Ray : public Object
	{
	public:
		Ray() {}
		Ray(const Vector3& vecOrigin, const Vector3& vecDir) :
			origin_(vecOrigin), direction_(vecDir)
		{
		}

		void SetDirection(const Vector3& vecDir) { direction_ = vecDir; }
		Vector3 GetDirection() { return direction_; }

		void SetOriginal(const Vector3& vecOrigin) { origin_ = vecOrigin; }
		const Vector3& GetOrigin()const { return origin_; }

		Vector3 GetPoint(float val)const { return origin_ + val * direction_; }
		float SqrtDistanceToPoint(const Vector3& vec)const;
		
	public:
		Vector3 origin_;
		Vector3 direction_;

		//DECLEAR_ENGINE_CLASS(Ray);
	};
}