#include "Quaternion.h"
#include "RuntimePrefix.h"
#include <limits>

namespace LightningEngine
{
	void Quaternion::Set(float x, float y, float z, float w)
	{
		x_ = x; y_ = y; z_ = z; w_ = w;
	}

	Quaternion EulerToQuaternion(const Vector3 & euler)
	{
		float cx(cos(euler.x_ / 2.0f));
		float sx(sin(euler.x_ / 2.0f));

		float cy(cos(euler.y_ / 2.0f));
		float sy(sin(euler.y_ / 2.0f));

		float cz(cos(euler.z_ / 2.0f));
		float sz(sin(euler.z_ / 2.0f));

		Quaternion qx(sx, 0.0f, 0.0f, cx);
		Quaternion qy(0.0f, sy, 0.0f, cy);
		Quaternion qz(0.0f, 0.0f, sz, cz);
		Quaternion ret = (qy * qx) * qz;
		return ret;
	}

	void QuaternionToMatrix(const Quaternion & q, Matrix3x3 & mat)
	{
		float x = q.x_ * 2.0f;
		float y = q.y_ * 2.0f;
		float z = q.z_ * 2.0f;
		float xx = q.x_ * x;
		float yy = q.y_ * y;
		float zz = q.z_ * z;
		float xy = q.x_ * y;
		float xz = q.x_ * z;
		float yz = q.y_ * z;
		float wx = q.w_ * x;
		float wy = q.w_ * y;
		float wz = q.w_ * z;

		mat.data_[0] = 1.0f - (yy + zz);
		mat.data_[1] = xy + wz;
		mat.data_[2] = xz - wy;

		mat.data_[3] = xy - wz;
		mat.data_[4] = 1.0f - (xx + zz);
		mat.data_[5] = yz + wx;

		mat.data_[6] = xz + wy;
		mat.data_[7] = yz - wx;
		mat.data_[8] = 1.0f - (xx + yy);
	}
}